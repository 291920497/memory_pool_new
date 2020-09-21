#include "memory_pool_example.h"

memory_pool_t* g_mp;

int eg_create_pool(size_t pool_size) {
	if (g_mp) {
		return 0;
	}

	if (g_mp = mp_create(pool_size)) {
		return 0;
	}
	return -1;
}

void eg_destroy_pool() {
	if (g_mp) {
		mp_destroy(g_mp);
		g_mp = 0;
	}
}

void* eg_malloc(size_t size) {
	return mp_malloc(g_mp, size);
}

void* eg_realloc(void*  block, size_t size) {
	return mp_realloc(g_mp, block, size);
}

void eg_free(void* block) {
	mp_free(g_mp, block);
}