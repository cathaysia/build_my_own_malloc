#pragma once

#define HACK_MALLOC

extern "C" {
#include <stddef.h>
}

extern "C" {
void* mem_alloc(size_t size);
void  mem_free(void* block);
void* mem_calloc(size_t num, size_t nsize);
void* mem_realloc(void* block, size_t size);
}
