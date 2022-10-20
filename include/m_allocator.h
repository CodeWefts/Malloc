#pragma once
#include <stddef.h>
 
// Memory functions
void* m_malloc(size_t size);
void* m_realloc(void* ptr, size_t size);
void* m_calloc(size_t nb, size_t size);
void m_free(void* ptr);
 
void m_setup_hooks(void); // Hook this allocator when using standard library

