/**
 * 
 * Structure to hold data about called functions, including total energy usage, 
 * function name, the (single) caller, and any callees.
 * 
 * Authors: @bensong04
 * 
*/

#pragma once

#include "uba.h"
#include <stdint.h>

typedef leaffn_t;

typedef struct leaffunction_header {
    uint64_t total_energy_usage;    
    leaffn_t caller;
    uba_t* callees; // pointer abuse: cast leaffn_t* to char* and back
    char* fn_ident;
} leaffn_t;

leaffn_t* new_leaffn(leaffn_t caller, char* fn_ident);

uint64_t get_total_energy_usage(leaffn_t* fn_node);

uint64_t increment_total_energy_usage(leaffn_t* fn_node, uint32_t energy_uj);

leaffn_t* get_caller(leaffn_t* fn_node);

uba_t* get_callees(leaffn_t* fn_node);

leaffn_t* find_callee(leaffn_t* fn_node, char* fn_ident);

void add_callee(leaffn_t* fn_node, leaffn_t* new_callee);

char* get_fn_ident(leaffn_t* fn_node);
