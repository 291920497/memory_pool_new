#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "memory_node.h"

#ifdef _WIN32
#include <Windows.h>
#else
//编译添加_GNU_SOURCE预处理
//#define _GNU_SOURCE
#include <pthread.h>
#endif//_WIN32

//测试
//#define MP_DEFAULT_ALLOC_SIZE (8192)

//默认单次申请长度 10M
#define MP_DEFAULT_ALLOC_SIZE (10485760)
#define MP_UNUSED_POOL_SIZE (20)

//非使用状态内存池1左移起始值
#define MP_UNUSED_BEGIN (6)
#define MP_UNUSED_END (6 + MP_UNUSED_POOL_SIZE - 1)
#define MP_SMALL_BLOCK ((1 << MP_UNUSED_BEGIN) - 1)

#ifdef __cplusplus
extern "C" {
#endif

/*
	unused_node:
		[0]: 保存了最小分割内存块,本例中为 ((1 << 6) - 1) <=> MP_SMALL_BLOCK 
		[1]: 保存了随机大小为 [ (1 << 7),(1 << 8)  ) 的内存块, 以保证申请大小区间为( (1 << 6), (1 << 7) )满足需要
		[.]: 往后以此类推，以空间换时间的方式分配足够大,随机大小的内存块，
		[19]:作为公共内存块，用于分割
		单次最大申请大小为 [ 1 << MP_UNUSED_END - 1, 1 << MP_UNUSED_END)
*/

typedef struct memory_pool {												
	memory_node_t* unused_node[MP_UNUSED_POOL_SIZE];						
	uint64_t thread_id;					//内存池所属线程id
	struct memory_pool* next;			//保留用于多线程内存池集成使用

#ifdef _WIN32
	CRITICAL_SECTION mp_unused_lock;
	//CRITICAL_SECTION mp_inused_lock;	//本例中不再关心使用中的内存块,这将导致消耗额外的时间片且没有实际的意义
#else
	pthread_spinlock_t mp_unused_lock;
	//pthread_spinlock_t mp_inused_lock;
#endif
}memory_pool_t;

memory_pool_t* mp_create(uint32_t init_size);

void mp_destroy(memory_pool_t* mp);

void* mp_malloc(memory_pool_t* mp, size_t size);

void mp_free(memory_pool_t* mp, void* free_ptr);

#ifdef __cplusplus
}
#endif


#endif//_MEMORY_POOL_H_