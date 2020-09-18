#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <stdint.h>

/*�Ƿ���������ģʽ���鿴ͷ�ڵ���Ϣ*/
//#define MP_DEBUG
/*Ĭ�������С����ֵӰ��������ڴ�鲻��ʱ�����Ĭ�ϴ�С*/
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