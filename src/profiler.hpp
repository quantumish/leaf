#pragma once

#include <unistd.h>
#include <stdint.h>

typedef std::vector<std::tuple<std::string, intptr_t, std::string>> call_stack_t;

uint32_t freeze(pid_t pid, uint32_t& last_rapl);

void unfreeze(pid_t pid);
call_stack_t unwind(pid_t target);
