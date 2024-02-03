#pragma once

#include <unistd.h>
#include <stdint.h>

<<<<<<< HEAD
typedef std::vector<std::tuple<std::string, intptr_t, std::string>> call_stack_t;

uint32_t freeze(pid_t pid, uint32_t& last_rapl);
=======
uint32_t freeze(pid_t pid, uint32_t* last_rapl);
>>>>>>> 55f59598e9740e977b2f92b12a209e7dbb55f2c2
void unfreeze(pid_t pid);
call_stack_t unwind(pid_t target);
