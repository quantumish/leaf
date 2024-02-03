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

#include "uba.h"

typedef struct flametree_header flametree_t;

flametree_t* flametree_new();

void flametree_free(flametree_t* root);

void flametree_update(flametree_t* root, uba_t call_stack, uint32_t energy_uj);

void flametree_dump(flametree_t* root, FILE* outstream);
