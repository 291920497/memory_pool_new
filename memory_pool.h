#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <stdint.h>

/*是否启动调试模式，查看头节点信息*/
//#define MP_DEBUG
/*默认申请大小，该值影响待分配内存块不足时申请的默认大小*/
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