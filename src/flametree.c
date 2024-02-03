#include "flametree.h"
#include <string.h>

flametree_t* flametree_new() {

    return NULL; // TODO
}

void flametree_update(flametree_t* root, uba_t* call_stack, uint32_t energy_uj) {
    size_t num_fns_in_stack = uba_len(call_stack);
    flametree_t* curr_leaf = root;
    increment_total_energy_usage(curr_leaf, energy_uj);

    // start at 1; first in callstack is always root entry point
    for (size_t i = 1; i < num_fns_in_stack; i++) {
        char* call_stack_fn_id = uba_get(call_stack, i); // get current node
        leaffn_t* next_callee = find_callee(curr_leaf, call_stack_fn_id);
        if (next_callee == NULL) {
            // create new node for callee
            // add callee to current node
            add_callee(curr_leaf, new_leaffn(curr_leaf, call_stack_fn_id));
        } 
        increment_total_energy_usage(curr_leaf, energy_uj);
    }
    return;
}

void flametree_dump(flametree_t* root, FILE* outstream) {}
