#pragma once

#include <unistd.h>
#include <stdint.h>

#include "uba.h"

uint32_t freeze(pid_t pid);
void unfreeze(pid_t pid);
uba_t* unwind(pid_t target);
