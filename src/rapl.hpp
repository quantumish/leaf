/**
 * 
 * Interface for reading energy usage data from RAPL.
 * 
 * @author bensong04, sim15
 * 
*/

#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(s) sleep(s)
#else
#include <time.h>
#define SLEEP(s) nanosleep((const struct timespec[]){{0, s * 1000000}}, NULL)
#endif

#ifdef __linux__
#define ENERGY_UJ_PATH "/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj"
#else
#define ENERGY_UJ_PATH "./dummy/dummy_rapl.txt"
#endif

uint32_t get_curr_mjoule_usage(uint32_t& last_rapl);

#ifdef NVIDIA
uint64_t gpu_uJ_since_ever();
#endif
