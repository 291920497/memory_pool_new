#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "memory_node.h"

#ifdef _WIN32
#include <Windows.h>
#else
//�������_GNU_SOURCEԤ����
//#define _GNU_SOURCE
#include <pthread.h>
#endif//_WIN32

//����
//#define MP_DEFAULT_ALLOC_SIZE (8192)

//Ĭ�ϵ������볤�� 10M
#define MP_DEFAULT_ALLOC_SIZE (10485760)
#define MP_UNUSED_POOL_SIZE (20)

//��ʹ��״̬�ڴ��1������ʼֵ
#define MP_UNUSED_BEGIN (6)
#define MP_UNUSED_END (6 + MP_UNUSED_POOL_SIZE - 1)
#define MP_SMALL_BLOCK ((1 << MP_UNUSED_BEGIN) - 1)

#ifdef __cplusplus
extern "C" {
#endif

/*
	unused_node:
		[0]: ��������С�ָ��ڴ��,������Ϊ ((1 << 6) - 1) <=> MP_SMALL_BLOCK 
		[1]: �����������СΪ [ (1 << 7),(1 << 8)  ) ���ڴ��, �Ա�֤�����С����Ϊ( (1 << 6), (1 << 7) )������Ҫ
		[.]: �����Դ����ƣ��Կռ任ʱ��ķ�ʽ�����㹻��,�����С���ڴ�飬
		[19]:��Ϊ�����ڴ�飬���ڷָ�
		������������СΪ [ 1 << MP_UNUSED_END - 1, 1 << MP_UNUSED_END)
*/

typedef struct memory_pool {												
	memory_node_t* unused_node[MP_UNUSED_POOL_SIZE];						
	uint64_t thread_id;					//�ڴ�������߳�id
	struct memory_pool* next;			//�������ڶ��߳��ڴ�ؼ���ʹ��

#ifdef _WIN32
	CRITICAL_SECTION mp_unused_lock;
	//CRITICAL_SECTION mp_inused_lock;	//�����в��ٹ���ʹ���е��ڴ��,�⽫�������Ķ����ʱ��Ƭ��û��ʵ�ʵ�����
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