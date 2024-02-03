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
 * Authors: @bensong04, @quantumish, @sim15
 * 
*/

#pragma once

#include <stdint.h>
#include <stdio.h>

#include <vector>
#include <string>

#include <iostream>
#include <fstream>

#include "leaffunction.hpp"
#include "json.h"

typedef struct flametree_header {
    leaffn_t* root_leaffn;
} flametree_t;

flametree_t* flametree_new();

void flametree_free(flametree_t* root);

typedef std::vector<std::tuple<std::string, intptr_t, std::string>> call_stack_t;
void flametree_update(flametree_t* root, call_stack_t call_stack, uint32_t energy_uj);

void flametree_dump(leaffn_t* root, int depth);

void flametree_dump_json(flametree_t* root, std::ofstream& outstream);
