#ifndef _MEMORY_POOL_EXAMPLE_H_
#define _MEMORY_POOL_EXAMPLE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
int eg_create_pool(size_t pool_size);

void eg_destroy_pool();

void* eg_malloc(size_t size);

void* eg_realloc(void*  block, size_t size);

void eg_free(void* block);

#ifdef __cplusplus
}
#endif

#endif//_MEMORY_POOL_EXAMPLE_H_