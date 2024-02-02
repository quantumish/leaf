#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <libunwind.h>
#include <libunwind-ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "c-hashmap/map.h"

#define MAX_SYMLEN 128

void unwind(pid_t pid, hashmap* hmap) {    
    ptrace(PTRACE_ATTACH, pid);
    kill(pid, SIGSTOP);
    waitpid(pid, NULL, 0);
    void* ui = _UPT_create(pid);
    unw_cursor_t c;
    unw_addr_space_t as = unw_create_addr_space(&_UPT_accessors, 0);
    unw_init_remote(&c, as, ui);
    do {
        unw_word_t offset;
        char* fname = calloc(MAX_SYMLEN, sizeof(char));
        int resp = unw_get_proc_name(&c, fname, MAX_SYMLEN, &offset);
        int count = 1;
        uintptr_t existing = 0;
        hashmap_get(hmap, fname, strlen(fname), &existing);
        count += (int)existing;
        hashmap_set(hmap, fname, strlen(fname), count);
    } while(unw_step(&c) > 0);
    _UPT_resume(as, &c, ui);
    _UPT_destroy(ui);
    kill(pid, SIGSTOP);
    kill(pid, SIGCONT);
    waitpid(pid, NULL, 0);
    ptrace(PTRACE_DETACH, pid, NULL, NULL);

}

void print_entry(void* key, size_t ksize, uintptr_t value, void* usr)
{
	// prints the entry's key and value
	// assumes the key is a null-terminated string
	printf("Entry \"%s\": %i\n", (char*)key, (int)value);
}



int main(int argc, char** argv) {
    pid_t target = strtoll(argv[1], NULL, 10);
    hashmap* m = hashmap_create();
    for (int i = 0; i < 10000; i++) {
        unwind(target, m);
        usleep(100);
    }
    hashmap_iterate(m, print_entry, NULL);
    
}
