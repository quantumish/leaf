/**
 * 
 * Structure for recording and traversing through hierarchal function calls. 
 * 
 * Everything is heap-allocated! :(
 * 
 * Author: @bensong04 
 * 
*/

#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct flametree_header flametree_t;

flametree_t* new_flametree();

void free_flametree(flametree_t* root_node);

void add_energy_to_flametree(flametree_t* root_node, uba_t call_stack, size_t uba_len, uint32_t energy_in_mj);

void write_tree_to_stream(flametree_t* root_node, FILE* outstream);