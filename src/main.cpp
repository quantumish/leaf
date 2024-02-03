#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <vector>
#include <string>

#include "flametree.hpp"
#include "profiler.hpp"

#include <iostream>

#define SLEEP_TIME 1000

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: ./leaf [pid]\n");
        exit(0);
    }
    pid_t target = strtoll(argv[1], NULL, 10);
    flametree_t* ft = flametree_new();
    // TODO do not make iteration based
    for (int i = 0; i < 10000; i++) {
        uint32_t energy = freeze(target);
        std::vector<std::string> stack = unwind(target);
        // for (auto s : stack) {
        // }
        /* for (int i = 0; i < uba_len(stack); i++) { */
        /*     printf("%s\n", uba_get(stack, i)); */
        /* } */
        /* printf("\n"); */
        unfreeze(target);
        flametree_update(ft, stack, energy);
        usleep(SLEEP_TIME);
    }
    /* FILE* out = fopen("./test.json", "w"); */
    flametree_dump(ft->root_leaffn, 0);
    /* fclose(out); */
}
