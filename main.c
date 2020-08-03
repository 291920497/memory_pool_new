#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include <stdio.h>

#include <stdlib.h>

#include "memory_pool.h"

int main(int argc,char** argv) {
	if (argc < 2) {
		printf("exe 1 or n\n");
		return 0;
	}

	int type = atoi(argv[1]);
	if (type == 1) {
		memory_pool_t* mp = mp_create(MP_DEFAULT_ALLOC_SIZE * 20);

		for (int i = 0; i < 1000000; ++i) {
			void* a = mp_malloc(mp, 100);
			//mp_free(mp, a);
		}
		mp_destroy(mp);
	}
	else {
		for (int i = 0; i < 1000000; ++i) {
			void* a = malloc(100);
			//free(a);
		}
	}
	return 0;
}