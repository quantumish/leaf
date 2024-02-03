/**
 * 
 * @author: sim15
 * 
*/

#include "flametree.hpp"
#include <string.h>

#include <iostream>

flametree_t* flametree_new() {
    flametree_t* res = (flametree_t*)malloc(sizeof(flametree_t));
    res->root_leaffn = new_leaffn(NULL, std::string("_start"));
    return res;
}


void flametree_free(flametree_t* root) {
    free_leaffn(root->root_leaffn);
    free(root);
    return;
}

void flametree_update(flametree_t* root, std::vector<std::string> call_stack, uint32_t energy_uj) {
    // CALL STACK
    size_t num_fns_in_stack = call_stack.size();

    // FLAMETREE
    leaffn_t* curr_leaf = root->root_leaffn;
    increment_total_energy_usage(curr_leaf, energy_uj);

    // start at 1; first in callstack is always root entry point
    for (size_t i = 1; i < num_fns_in_stack; i++) {
        std::string call_stack_fn_id = call_stack[i]; // get current node
        leaffn_t* next_callee = find_callee(curr_leaf, call_stack_fn_id);
        increment_total_energy_usage(curr_leaf, energy_uj);
        curr_leaf = next_callee;
    }
    return;
}

void flametree_dump(leaffn_t* root, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    std::cout << root->total_energy_usage <<  " " << root->fn_ident << "\n";
    // std::cout << root->callees.size() << "\n";
    for (auto child : root->callees) {
        flametree_dump(child.second, depth+1);
    }     
}
