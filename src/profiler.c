#include <stdint.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <libunwind.h>
#include <libunwind-ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"
#include "uba.h"
#include "rapl.h"

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

uba_t* unwind(pid_t pid) {    
    void* ui = _UPT_create(pid);
    unw_cursor_t c;
    unw_addr_space_t as = unw_create_addr_space(&_UPT_accessors, 0);
    unw_init_remote(&c, as, ui);
    uba_t* stack = uba_new(8, MAX_SYMLEN+1);    
    do {
        unw_word_t offset;
        char fname[MAX_SYMLEN] = {0};
        int resp = unw_get_proc_name(&c, fname, MAX_SYMLEN, &offset);
        uba_add(stack, fname);                        
    } while(unw_step(&c) > 0);
    _UPT_resume(as, &c, ui);
    _UPT_destroy(ui);
    unw_destroy_addr_space(as);    
    return stack;
}
