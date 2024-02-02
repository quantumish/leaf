/**
 * 
 * Interface for reading energy usage data from RAPL.
 * 
 * Authors: @bensong04
 * 
*/

#pragma once

#include <stdint.h>

#ifndef __APPLE__
#define ENERGY_UJ_PATH "/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj"
#else
#define ENERGY_UJ_PATH "./dummy/dummy_rapl.txt"
#endif

uint32_t get_curr_mjoule_usage();