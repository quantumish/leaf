#pragma once

#include <unistd.h>
#include <stdint.h>

uint32_t freeze(pid_t pid);
void unfreeze(pid_t pid);
std::vector<std::string> unwind(pid_t target);