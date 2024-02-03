/**
 * 
 * @author: sim15
 * 
*/

#include "flametree.hpp"
#include <string.h>
#include <stack>

#include <iostream>
#include <fstream>

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

void _dump_dfs_helper(leaffn_t* curr_node, json::jobject& curr_json_level) {
    std::unordered_map<std::string, leaffn_t*> node_callees = 
                    get_callees(curr_node);
    std::vector<json::jobject> curr_children;
    for (auto callee = node_callees.begin(); callee != node_callees.end(); ++callee) {
        json::jobject new_callee_json_level;
        _dump_dfs_helper(callee->second, new_callee_json_level);
        curr_children.push_back(new_callee_json_level);        
    }    
    curr_json_level["children"] = curr_children;
    curr_json_level["ident"] = get_fn_ident(curr_node);
    curr_json_level["energy"] = (unsigned long) get_total_energy_usage(curr_node);
}

void flametree_dump_json(flametree_t* root, std::ofstream& outstream) {
    json::jobject master_json_object;
    _dump_dfs_helper(root->root_leaffn, master_json_object);
    outstream << (std::string) master_json_object << '\n';
}
