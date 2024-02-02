/**
 * 
 * Interface for reading energy usage data from RAPL.
 * 
 * Authors: @bensong04
 * 
**/

#pragma once

#include <stdint.h>

uint32_t get_curr_mjoule_usage();