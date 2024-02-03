/**
 * 
 * Author: @bensong04
 * 
*/

#include <stdlib.h>
#include <string.h>

#include <vector>
#include <string>

#include "leaffunction.hpp"

#define UBA_DEFAULT_SIZE 10

leaffn_t* new_leaffn(leaffn_t* caller, char* fn_ident) {
    leaffn_t* new_leaf = malloc(sizeof(leaffn_t));
    if (new_leaf == NULL) return NULL; // allocating memory for the header failed
    new_leaf->total_energy_usage = 0;
    new_leaf->caller = caller;
    new_leaf->fn_ident = (char*)malloc(strlen(fn_ident) + 1); // allocate enough memory for the
                                                     // null terminating byte
    if (new_leaf->fn_ident == NULL) { // allocating memory for the identifier failed 
        free(new_leaf); // free the header
        return NULL;
    }
    strncpy(new_leaf->fn_ident, fn_ident, strlen(fn_ident));
    new_leaf->fn_ident[strlen(fn_ident)] = '\0'; // manually setting the terminating byte
    // new_leaf->callees = uba_new(UBA_DEFAULT_SIZE, sizeof(leaffn_t));
    // if (new_leaf->callees == NULL) {
    //     free(new_leaf->fn_ident);
    //     free(new_leaf);
    //     return NULL;
    // } 
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

std::vector<leaffn_t*> get_callees(leaffn_t* fn_node) {
    return fn_node->callees;    
}

char* get_fn_ident(leaffn_t* fn_node) {
    return fn_node->fn_ident;   
}

leaffn_t* find_callee(leaffn_t* fn_node, char* fn_ident) {
    std::vector<leaffn_t*> callees = fn_node->callees;
    size_t num_callees = callees.size();
    for (size_t callee_idx = 0; callee_idx < num_callees; callee_idx++) {
        // TODO
    }
}

void add_callee(leaffn_t* fn_node, leaffn_t* new_callee) {    
    fn_node->callees.push_back(new_callee);
}

void free_leaffn(leaffn_t* fn_node) {
    uba_free(fn_node->callees);
    free(fn_node->fn_ident);
    free(fn_node);
}
