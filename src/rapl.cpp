#include <stdio.h>
#include "rapl.hpp"

uint32_t get_curr_mjoule_usage() {
    FILE* energy_uj_data = fopen(ENERGY_UJ_PATH, "r");
    if (energy_uj_data == NULL) return UINT32_MAX;
    uint32_t usage_in_mj = UINT32_MAX;
    fscanf(energy_uj_data, "%u", &usage_in_mj);
    fclose(energy_uj_data);
    return usage_in_mj;
}

