#include "flametree.h"

flametree_t* flametree_new() {

    return NULL; // TODO
}

void flametree_update(flametree_t* root, uba_t* call_stack, uint32_t energy_uj) {
    size_t num_fns_in_stack = uba_len(call_stack);
    return;
}

void flametree_dump(flametree_t* root, FILE* outstream) {}
