/**
 * 
 * Structure to hold data about called functions, including total energy usage, 
 * function name, the (single) caller, and any callees.
 * 
 * Authors: @bensong04
 * 
*/

#pragma once

#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <string>

typedef struct leaffunction_header {
    uint64_t total_energy_usage;    
    struct leaffunction_header* caller;
    std::unordered_map<std::string, struct leaffunction_header*> callees; 
    std::string fn_ident;
} leaffn_t;

leaffn_t* new_leaffn(leaffn_t* caller, std::string fn_ident);

uint64_t get_total_energy_usage(leaffn_t* fn_node);

uint64_t increment_total_energy_usage(leaffn_t* fn_node, uint32_t energy_uj);

leaffn_t* get_caller(leaffn_t* fn_node);

std::unordered_map<std::string, struct leaffunction_header*> get_callees(leaffn_t* fn_node);

leaffn_t* find_callee(leaffn_t* fn_node, std::string fn_ident);

void add_callee(leaffn_t* fn_node, leaffn_t* new_callee);

std::string get_fn_ident(leaffn_t* fn_node);

void free_leaffn(leaffn_t* fn_node);
