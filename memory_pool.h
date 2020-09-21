#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <stdio.h>

#ifndef _WIN32
#include <glob.h>
#endif//_WIN32

/*
	是否启动调试模式，查看头节点信息
*/
//#define MP_DEBUG

/*	
	是否启用单线程模式,即便是单线程使用原子锁，也将带来时间的损耗，即便很小
	但确定在单线程环境下使用，依然建议启动该预处理
*/
//#define MP_SINGLE_THREAD_MOD

/*
	默认申请大小，该值影响待分配内存块不足时申请的默认大小
*/
#define MP_DEFAULT_ALLOC_SIZE (10485760)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct memory_pool memory_pool_t;
memory_pool_t* mp_create(size_t pool_size);

void mp_destroy(memory_pool_t* mp);

void* mp_malloc(memory_pool_t* mp, size_t size);

void* mp_realloc(memory_pool_t* mp, void* block, size_t size);

void mp_free(memory_pool_t* mp, void* free_ptr);

#ifdef __cplusplus
}
#endif

#endif//_MEMORY_POOL_H_