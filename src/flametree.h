/**
 * 
 * Structure for recording and traversing through hierarchal function calls. 
 * 
 * Note: this is an acyclic graph (minus the double-linkage). There may be multiple 
 * nodes in the stack with the same name. 
 * Ex. here is the flametree after f1 calls f2 and f3 calls f2:
 * 
 *  +---> f1 --> f2
 * main
 *  +---> f3 --> f2 
 * 
 * Everything is heap-allocated! :(
 * 
 * Authors: @bensong04, @quantumish
 * 
*/

#pragma once

#include <stdint.h>
#include <stdio.h>

#include "uba.h"
#include "leaffunction.h"

typedef struct flametree_header {
        
} flametree_t;

flametree_t* flametree_new();

void flametree_free(flametree_t* root);

void flametree_update(flametree_t* root, uba_t* call_stack, uint32_t energy_uj);

void flametree_dump(flametree_t* root, FILE* outstream);
