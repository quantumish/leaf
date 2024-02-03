#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "rapl.hpp"
#ifdef NVIDIA
#include <nvml.h>
#endif

uint32_t get_curr_mjoule_usage(uint32_t& last_rapl) {
    int energy_uj_data = open(ENERGY_UJ_PATH, O_RDONLY);
    char buf[16] = {0};
    read(energy_uj_data, buf, 16);    
    uint32_t usage_in_mj = UINT32_MAX;
    std::sscanf(buf, "%u", &usage_in_mj);
    close(energy_uj_data);
    uint32_t out = usage_in_mj - last_rapl;
    if (out < 0) out = UINT32_MAX + out; // account for overflow
    last_rapl = usage_in_mj;
    return out;
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
