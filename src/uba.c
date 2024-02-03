#include <stdlib.h>
#include <string.h>

#include "uba.h"

uba_t* uba_new(size_t sz, size_t elt_sz) {
    uba_t* out = malloc(sizeof(struct uba));
    out->size = 0;
    out->capacity = sz;
    out->elt_size = elt_sz;
    out->data = calloc(elt_sz * sz, sizeof(char));
    return out;
}

char* uba_get(uba_t* uba, size_t i) {
    if (i >= uba->size) return NULL;
    return uba->data + (uba->elt_size * i);
}

void uba_resize(uba_t* uba) {
    uba->capacity *= 2;
    // TODO handle errors
    realloc(uba->data, uba->capacity * 2);
}

void uba_add(uba_t* uba, char *s) {
    if (uba->size == uba->capacity) uba_resize(uba);
    strcpy(uba->data+(uba->size*uba->elt_size), s);
    uba->size++;
}

size_t uba_len(uba_t* uba) {
    return uba->size;
}

void uba_free(uba_t* uba) {
    free(uba->data);
    free(uba);
}
