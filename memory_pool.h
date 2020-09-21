#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <stdio.h>

#ifndef _WIN32
#include <glob.h>
#endif//_WIN32

/*
	�Ƿ���������ģʽ���鿴ͷ�ڵ���Ϣ
*/
//#define MP_DEBUG

/*	
	�Ƿ����õ��߳�ģʽ,�����ǵ��߳�ʹ��ԭ������Ҳ������ʱ�����ģ������С
	��ȷ���ڵ��̻߳�����ʹ�ã���Ȼ����������Ԥ����
*/
//#define MP_SINGLE_THREAD_MOD

/*
	Ĭ�������С����ֵӰ��������ڴ�鲻��ʱ�����Ĭ�ϴ�С
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