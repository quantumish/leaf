#include <stdio.h>
#include "rapl.hpp"
#ifdef NVIDIA
#include <nvml.h>
#endif

uint32_t get_curr_mjoule_usage() {
    FILE* energy_uj_data = fopen(ENERGY_UJ_PATH, "r");
    if (energy_uj_data == NULL) return UINT32_MAX;
    uint32_t usage_in_mj = UINT32_MAX;
    fscanf(energy_uj_data, "%u", &usage_in_mj);
    fclose(energy_uj_data);
    return usage_in_mj;
}

#ifdef NVIDIA
uint64_t gpu_uJ_since_ever() {
	nvmlInit_v2();
	nvmlDevice_t dev;
	nvmlDeviceGetHandleByIndex(0, &dev);
	uint64_t energy;
	nvmlReturn_t ret = nvmlDeviceGetTotalEnergyConsumption(dev, reinterpret_cast<unsigned long long int*>(&energy));
	return energy*1000;
}
#endif
