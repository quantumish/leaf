#pragma once

#include <stddef.h>

typedef struct uba {
    char* data;
    size_t size;
    size_t elt_size;
    size_t capacity;
}* uba_t;

uba_t uba_new(size_t sz, size_t elt_sz);

// Returns ith element of UBA, NULL if invalid index.
char* uba_get(uba_t uba, size_t i);

void uba_resize(uba_t uba);
void uba_add(uba_t uba, char* s);
size_t uba_len(uba_t uba);

