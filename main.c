#include "memory_node.h"
#include <stdio.h>
#include <stdlib.h>

#include "memory_pool.h"

int main() {
	memory_pool_t* mp = mp_create(10 * 1024 * 1024);

	memory_node_t* mn;
	mn = mp_malloc(mp, 15);
	mn = mp_realloc(mp, mn, 22);
	mn = mp_realloc(mp, mn, 33);
	mp_free(mp, mn);
	mn = mp_malloc(mp, 32);
	mp_free(mp, mn);
	mn = mp_malloc(mp, 55);
	mp_free(mp, mn);
	mn = mp_malloc(mp, 64);
	mp_destroy(mp);
}
