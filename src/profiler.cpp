#include <Zydis/Disassembler.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <libunwind.h>
#include <libunwind-ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <vector>
#include <string>

#include "constants.hpp"
#include "rapl.hpp"

#include <sstream>

extern "C" {
#include <Zydis/Zydis.h>
#include <Zydis/Disassembler.h>
}

uint32_t freeze(pid_t pid, uint32_t& last_rapl) {
    ptrace(PTRACE_ATTACH, pid);
    kill(pid, SIGSTOP);
    waitpid(pid, NULL, 0);
    return get_curr_mjoule_usage(last_rapl);
}

void unfreeze(pid_t pid) {
    kill(pid, SIGSTOP);
    kill(pid, SIGCONT);
    // waitpid(pid, NULL, 0);
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

std::string disas(int pid, unsigned long addr, unsigned long cur_addr)
{
    std::stringstream ss;
    ZyanU64 runtime_address = addr;
    ZyanUSize offset = 0;
    ZydisDisassembledInstruction instruction;
    long ins[8];
    ins[0] = ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);
    ins[1] = ptrace(PTRACE_PEEKTEXT, pid, addr+8, NULL);
    ins[2] = ptrace(PTRACE_PEEKTEXT, pid, addr+16, NULL);
    ins[3] = ptrace(PTRACE_PEEKTEXT, pid, addr+24, NULL);
    ins[4] = ptrace(PTRACE_PEEKTEXT, pid, addr+32, NULL);
    ins[5] = ptrace(PTRACE_PEEKTEXT, pid, addr+40, NULL);
    ins[6] = ptrace(PTRACE_PEEKTEXT, pid, addr+48, NULL);
    ins[7] = ptrace(PTRACE_PEEKTEXT, pid, addr+56, NULL);
    
    ZyanU8* data = (ZyanU8*)&ins;
    while (ZYAN_SUCCESS(ZydisDisassembleATT(
                                            /* machine_mode:    */ ZYDIS_MACHINE_MODE_LONG_64,
                                            /* runtime_address: */ runtime_address,
                                            /* buffer:          */ data + offset,
                                            /* length:          */ sizeof(data)*16 - offset,
                                            /* instruction:     */ &instruction
                                            ))) {
        ss << runtime_address << "\t" << instruction.text << "\n";             
        // printf("=> %016" PRIX64 "  %s\n", runtime_address, instruction.text);
        // } else printf("%016" PRIX64 "  %s\n", runtime_address, instruction.text);
        offset += instruction.info.length;
        runtime_address += instruction.info.length;
    }
    return ss.str();
}


std::vector<std::tuple<std::string, intptr_t, std::string>> unwind(pid_t pid) {    
    void* ui = _UPT_create(pid);
    unw_cursor_t c;
    unw_addr_space_t as = unw_create_addr_space(&_UPT_accessors, 0);
    unw_init_remote(&c, as, ui);
    std::vector<std::tuple<std::string, intptr_t, std::string>> stack {};
    do {
        unw_word_t offset;
        char fname[MAX_SYMLEN] = {0};
        int resp = unw_get_proc_name(&c, fname, MAX_SYMLEN, &offset);
        unw_proc_info_t pip;
        unw_get_proc_info(&c, &pip);
        unw_word_t ip;
        unw_get_reg(&c, UNW_REG_IP, &ip);
        std::string dis = disas(pid, pip.start_ip, ip); 

        
        // Dl_info info;
        // dladdr((void*)ip, &info);
        // // printf("%p\n", (void*)ip);        
        // printf("%s: %p\n",fname, (char*)info.dli_fbase);       

        // disas(pid, ip); 
        // printf("RIP: %llx instruction %lx\n", regs.rip, ins);
        stack.emplace_back(std::string(fname), ip, dis);
    } while(unw_step(&c) > 0);
    _UPT_resume(as, &c, ui);
    _UPT_destroy(ui);
    unw_destroy_addr_space(as);
    std::reverse(stack.begin(),stack.end());
    return stack;
}

