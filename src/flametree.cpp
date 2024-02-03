/**
 * 
 * @author: sim15
 * 
*/

#include "flametree.hpp"
#include "leaffunction.hpp"
#include <algorithm>
#include <cstdint>
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

void flametree_update(flametree_t* root, call_stack_t call_stack, uint32_t energy_uj) {
    // CALL STACK
    size_t num_fns_in_stack = call_stack.size();

    // FLAMETREE
    leaffn_t* curr_leaf = root->root_leaffn;
    increment_total_energy_usage(curr_leaf, energy_uj);

    // start at 1; first in callstack is always root entry point
    for (size_t i = 1; i < num_fns_in_stack; i++) {
        std::string call_stack_fn_id = std::get<0>(call_stack[i]); // get current node
        leaffn_t* next_callee = find_callee(curr_leaf, call_stack_fn_id);
        next_callee->disas = std::get<2>(call_stack[i]);

        intptr_t rip = std::get<1>(call_stack[i]);
        if (!next_callee->instrs.contains(rip)) { // hopefully true
            next_callee->instrs.insert({rip, 1});
        } else {
            next_callee->instrs[rip] += 1;
        }
    
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

void get_leaves(leaffn_t* root, std::vector<leaffn_t*>& leaves, int depth) {
    std::unordered_map<std::string, leaffn_t*>  node_callees = get_callees(root);
    leaves.push_back(root);
    for (auto callee : node_callees) {
        get_leaves(callee.second, leaves, depth+1);        
    } 
}

std::vector<leaffn_t*> worst(leaffn_t* root) {
    std::vector<leaffn_t*> leaves;
    get_leaves(root, leaves, 0);
    std::sort(leaves.begin(), leaves.end(), [](leaffn_t* a, leaffn_t*b) {
        return a->total_energy_usage > b->total_energy_usage;
    });
    std::vector<leaffn_t*> bad(leaves.begin()+3, leaves.begin()+8);
    return bad;
}

void _dump_dfs_helper(leaffn_t* curr_node, json::jobject& curr_json_level) {
    std::unordered_map<std::string, leaffn_t*>  node_callees = 
                    get_callees(curr_node);
    std::vector<json::jobject> curr_children;
    for (auto callee = node_callees.begin(); callee != node_callees.end(); ++callee) {
        json::jobject new_callee_json_level;
        _dump_dfs_helper(callee->second, new_callee_json_level);
        curr_children.push_back(new_callee_json_level);        
    }    
    curr_json_level["children"] = curr_children;
    curr_json_level["ident"] = get_fn_ident(curr_node);
    curr_json_level["disas"] = curr_node->disas;
    json::jobject jinstrs;
    std::unordered_map<intptr_t, unsigned> instrs = curr_node->instrs;
    for (auto const& [key,val] : instrs) {
        jinstrs[std::to_string(key)] = val;
    }
    curr_json_level["instrs"] = jinstrs;
    curr_json_level["energy"] = (unsigned long) get_total_energy_usage(curr_node);
}

void flametree_dump_json(flametree_t* root, std::ofstream& outstream) {
    json::jobject master_json_object;
    _dump_dfs_helper(root->root_leaffn, master_json_object);
    std::vector<json::jobject> worsts;
    for (auto w : worst(root->root_leaffn)) {
        json::jobject f;
        f["ident"] = get_fn_ident(w);
        f["disas"] = w->disas;
        json::jobject jinstrs;
        std::unordered_map<intptr_t, unsigned> instrs = w->instrs;
        for (auto const& [key,val] : instrs) {
            jinstrs[std::to_string(key)] = val;
        }
        f["instrs"] = jinstrs;
        f["energy"] = (unsigned long) get_total_energy_usage(w);
        worsts.push_back(f);
    }
    master_json_object["worst"] = worsts;
    outstream << (std::string) master_json_object << '\n';
}
