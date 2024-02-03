/**
 * 
 * @author: bensong04
 * 
*/

#include <stdlib.h>
#include <string.h>

#include <vector>
#include <string>

#include "leaffunction.hpp"

#define UBA_DEFAULT_SIZE 10

leaffn_t* new_leaffn(leaffn_t* caller, std::string fn_ident) {
    leaffn_t* new_leaf = new leaffn_t;
    if (new_leaf == NULL) return NULL; // allocating memory for the header failed
    new_leaf->callees = std::unordered_map<std::string, leaffn_t*>{};
    new_leaf->total_energy_usage = 0;
    new_leaf->caller = caller;
    new_leaf->fn_ident = fn_ident;
    return new_leaf;
}

uint64_t get_total_energy_usage(leaffn_t* fn_node) {
    return fn_node->total_energy_usage;
}

uint64_t increment_total_energy_usage(leaffn_t* fn_node, uint32_t energy_uj) {
    fn_node->total_energy_usage += energy_uj;
    return fn_node->total_energy_usage;
}

leaffn_t* get_caller(leaffn_t* fn_node) {
    return fn_node->caller;
}

std::unordered_map<std::string, leaffn_t*> get_callees(leaffn_t* fn_node) {
    return fn_node->callees;    
}

std::string get_fn_ident(leaffn_t* fn_node) {
    return fn_node->fn_ident;   
}

leaffn_t* find_callee(leaffn_t* fn_node, std::string fn_ident) {
    std::unordered_map<std::string, leaffn_t*> callees = fn_node->callees;
    if (!callees.contains(fn_ident)) { // hopefully true
        add_callee(fn_node, new_leaffn(fn_node, fn_ident));
    }
    else return callees[fn_ident];
}

void add_callee(leaffn_t* fn_node, leaffn_t* new_callee) {    
    fn_node->callees.insert({new_callee->fn_ident, new_callee});
}

void free_leaffn(leaffn_t* fn_node) {
    delete fn_node;
}
