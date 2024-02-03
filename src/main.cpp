#include <fstream>
#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <vector>
#include <string>

#include "flametree.hpp"
#include "profiler.hpp"

#include <iostream>
#include <fstream>
#include <dirent.h>

#define SLEEP_TIME 10

std::vector<pid_t> get_tids(pid_t target) {
    char path[32] = {0};
    std::snprintf(path, 32, "/proc/%d/tasks", target);
    printf("%s\n", path);
    struct dirent *ep;
    std::vector<pid_t> tids;
    DIR* dp = opendir(path);
    while ((ep = readdir(dp))) {
        tids.push_back(strtoll(ep->d_name, NULL, 10));
    }
    closedir(dp);
    return tids;    
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: ./leaf [pid]\n");
        exit(0);
    }
    pid_t target = strtoll(argv[1], NULL, 10);
    std::vector<pid_t> tids = get_tids(target);
    std::vector<flametree_t*> fts;
    for (pid_t t : tids) fts.push_back(flametree_new());  
    // TODO do not make iteration based
    for (int i = 0; i < 10000; i++) {
        for (int i = 0; i < tids.size(); i++) {        
            uint32_t energy = freeze(tids[i]);
#ifdef NVIDIA
            std::cout << gpu_uJ_since_ever() << "\n";
#endif 
            std::vector<std::string> stack = unwind(tids[i]);
            
            flametree_update(fts[i], stack, energy);
            unfreeze(tids[i]);
        }
        usleep(SLEEP_TIME);
    }
    // flametree_dump(ft->root_leaffn, 0);
    for (int i = 0; i < tids.size(); i++) {    
        std::ofstream ofs;
        char path[32] = {0};
        std::snprintf(path, 32, "./thread_%d.json", tids[i]);
        ofs.open(path, std::ofstream::out);
        flametree_dump_json(fts[i], ofs);
    }
    // FILE* out = fopen("./test.json", "w");
    // flametree_dump(ft->root_leaffn, 0);
    /* fclose(out); */
}
