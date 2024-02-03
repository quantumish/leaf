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

#include <dlfcn.h>
#include <udis86.h>

uint32_t freeze(pid_t pid) {
    ptrace(PTRACE_ATTACH, pid);
    kill(pid, SIGSTOP);
    waitpid(pid, NULL, 0);
    return get_curr_mjoule_usage();
}

void unfreeze(pid_t pid) {
    kill(pid, SIGSTOP);
    kill(pid, SIGCONT);
    waitpid(pid, NULL, 0);
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

int disas(int pid, unsigned long addr)
{
	ud_t ud_obj;
	unsigned char *buff;
    
    long ins = ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);
    buff = (unsigned char*)&ins;
    // printf("%lx (%x %x %x %x)\n", ins, buff[0], buff[1], buff[2], buff[3]);
    
	// if (ptrace(pid, addr, buff) == -1) {
	// 	printf("(Can't read)\n");
	// 	return -1;
	// }    

    int remaining = 8;
    while (remaining > 0) {
        ud_init(&ud_obj);
        ud_set_input_buffer(&ud_obj, buff+(8-remaining), 64-(8*(8-remaining)));
        ud_set_mode(&ud_obj, 64);
        ud_set_syntax(&ud_obj, UD_SYN_ATT);

        // ud_disassemble fills the ud_obj struct with data
        if (ud_disassemble(&ud_obj) != 0) {
            printf("%016lx %-20s %s\n", addr+(8-remaining),
                   ud_insn_hex(&ud_obj), ud_insn_asm(&ud_obj));
        } else {
            printf("idk bro\n");
        }
        remaining -= (int)ud_insn_len(&ud_obj);
    }
}


std::vector<std::string> unwind(pid_t pid) {    
    void* ui = _UPT_create(pid);
    unw_cursor_t c;
    unw_addr_space_t as = unw_create_addr_space(&_UPT_accessors, 0);
    unw_init_remote(&c, as, ui);
    std::vector<std::string> stack {};
    do {
        unw_word_t offset;
        char fname[MAX_SYMLEN] = {0};
        int resp = unw_get_proc_name(&c, fname, MAX_SYMLEN, &offset);
        unw_word_t ip;
        unw_get_reg(&c, UNW_REG_IP, &ip);
        Dl_info info;
        dladdr((void*)ip, &info);
        printf("%p\n", (void*)ip);        
        printf("%s: %p\n",fname, (char*)info.dli_fbase);        
        // disas(pid, regs.rip);
        // printf("RIP: %llx instruction %lx\n", regs.rip, ins);
        stack.emplace_back(fname);
    } while(unw_step(&c) > 0);
    _UPT_resume(as, &c, ui);
    _UPT_destroy(ui);
    unw_destroy_addr_space(as);
    std::reverse(stack.begin(),stack.end());
    return stack;
}

