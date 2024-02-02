#include <stdio.h>
#include "rapl.h"

#ifndef __APPLE__
#define ENERGY_UJ_PATH "/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj"
#else
#define ENERGY_UJ_PATH "./dummy/dummy_rapl.txt"
#endif

uint32_t get_curr_mjoule_usage() {
    FILE* energy_uj_data = fopen(ENERGY_UJ_PATH, "r");
    if (energy_uj_data == NULL) return UINT32_MAX;
    uint32_t usage_in_mj = UINT32_MAX;
    fscanf(energy_uj_data, "%u", &usage_in_mj);
    fclose(energy_uj_data);
    return usage_in_mj;
}

