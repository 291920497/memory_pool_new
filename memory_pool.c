#include "memory_pool.h"
#include "memory_node.h"

#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
//编译添加_GNU_SOURCE预处理
#include <pthread.h>
#endif//_WIN32

//define

#define MP_UNUSED_POOL_SIZE (20)

//非使用状态内存池1左移起始值

#define MP_UNUSED_BEGIN (5)
#define MP_UNUSED_END (MP_UNUSED_BEGIN + MP_UNUSED_POOL_SIZE - 1)

typedef struct memory_pool {
	memory_node_t* alloc_node;
	memory_node_t* unused_node[MP_UNUSED_POOL_SIZE];

#ifndef MP_SINGLE_THREAD_MOD

#ifdef _WIN32
	CRITICAL_SECTION mp_unused_lock;
#else
	pthread_spinlock_t mp_unused_lock;
#endif//_WIN32

#endif//MP_SINGLE_THREAD_MOD
}memory_pool_t;

//tailq critical section

static void mp_enter_unused_lock(memory_pool_t* mp) {
#ifndef MP_SINGLE_THREAD_MOD

#ifndef _WIN32
	pthread_spin_lock(&(mp->mp_unused_lock));
#else
	EnterCriticalSection(&(mp->mp_unused_lock));
#endif//_WIN32

#endif//MP_SINGLE_THREAD_MOD
}

static void mp_leave_unused_lock(memory_pool_t* mp) {
#ifndef MP_SINGLE_THREAD_MOD

#ifndef _WIN32
	pthread_spin_unlock(&(mp->mp_unused_lock));
#else
	LeaveCriticalSection(&(mp->mp_unused_lock));
#endif//_WIN32

#endif//MP_SINGLE_THREAD_MOD
}



memory_pool_t* mp_create(size_t pool_size) {
	memory_pool_t* mp = malloc(sizeof(memory_pool_t));
	if (mp) {
		//初始化
		memset(mp, 0, sizeof(memory_pool_t));
		mp->alloc_node = malloc(sizeof(memory_node_t) + pool_size);
		if (mp->alloc_node == 0) { goto mp_create_failed; }

		mp->alloc_node->size = pool_size;
		mp->alloc_node->pool_idx = 0;
		mp->alloc_node->tailq_next = 0;

#ifndef MP_SINGLE_THREAD_MOD

#ifdef _WIN32
		InitializeCriticalSection(&(mp->mp_unused_lock));
#else
		pthread_spin_init(&(mp->mp_unused_lock), PTHREAD_PROCESS_PRIVATE);
#endif//_WIN32

#endif//MP_SINGLE_THREAD_MOD
		return mp;
	}

mp_create_failed:

	if (mp) {
		free(mp);
	}
	return 0;
}

void mp_destroy(memory_pool_t* mp) {
	free(mp->alloc_node);
#ifndef MP_SINGLE_THREAD_MOD

#ifndef _WIN32
	pthread_spin_destroy(&(mp->mp_unused_lock));
#else
	DeleteCriticalSection(&(mp->mp_unused_lock));
#endif//_WIN32

#endif//MP_SINGLE_THREAD_MOD
	free(mp);
}


void* mp_malloc(memory_pool_t* mp, size_t size) {
	int bit_idx;			//申请的目标索引
	int new_size;			//重新分配的大小
	int alloc_size;			//本次分配的有边界
	int left_idx = MP_UNUSED_BEGIN;
	int right_idx = MP_UNUSED_END;
	memory_node_t* mn;		

	//返回位运算的右边界，采用分治，否则过大的右边界将增加运算时间

	//乐观期望
	if ((bit_idx = bit_range2(left_idx, right_idx / 2, size)) == -1) {
		//悲观期望
		if ((bit_idx = bit_range2(right_idx / 2, right_idx, size)) == -1) {
			goto mp_malloc_failed;
		}
	}

	//记录申请的大小
	alloc_size = 1 << bit_idx;
	bit_idx -= left_idx;

	//开始分配

	//若有现成的
#ifndef MP_SINGLE_THREAD_MOD
	mp_enter_unused_lock(mp);
#endif//MP_SINGLE_THREAD_MOD
	if (mn = mp->unused_node[bit_idx]) {
		mp->unused_node[bit_idx] = mn->tailq_next;
		mn->use_size = size;
		goto mp_malloc_complate;
	}

	//若没有现成的
mp_malloc_try_agin:
	mn = cutting_tail(mp->alloc_node, alloc_size);
	if (mn) {
		mn->pool_idx = bit_idx;
		mn->use_size = size;
		goto mp_malloc_complate;
	}

	//申请合适的内存块
	new_size = MP_DEFAULT_ALLOC_SIZE + sizeof(memory_node_t) * 2;
	if ((1 << (bit_idx + left_idx)) > MP_DEFAULT_ALLOC_SIZE) {
		new_size = (1 << (bit_idx + left_idx)) + sizeof(memory_node_t) * 2;
	}

	mn = malloc(new_size);
	if (mn == 0) { goto mp_malloc_complate; }

	//初始化并加入未分配队列
	mn->use_size = 0;
	mn->size = new_size - sizeof(memory_node_t);
	mn->tailq_next = mp->alloc_node;
	mp->alloc_node = mn;
	goto mp_malloc_try_agin;
	
mp_malloc_complate:
#ifndef MP_SINGLE_THREAD_MOD
	mp_leave_unused_lock(mp);
#endif//MP_SINGLE_THREAD_MOD

#ifdef MP_DEBUG
	return mn;
#else
	return ++mn;
#endif//MP_DEBUG

mp_malloc_failed:
	return 0;
}

void* mp_realloc(memory_pool_t* mp, void* block, size_t size) {
	memory_node_t* new_block;
	memory_node_t* mn = block;
#ifndef MP_DEBUG
	--mn;
#endif//MP_DEBUG

	if (size <= mn->size) {
		mn->use_size = size;
		return block;
	}

	new_block = mp_malloc(mp, size);
	if (new_block) {
#ifndef MP_DEBUG
		--new_block;
#endif//MP_DEBUG
		memcpy(new_block + 1, mn + 1, mn->use_size);
	}

	mp_free(mp, block);

#ifndef MP_DEBUG
	return ++new_block;
#else
	return new_block;
#endif//MP_DEBUG
}

void mp_free(memory_pool_t* mp, void* free_ptr) {
	memory_node_t* mn = free_ptr;
#ifndef MP_DEBUG
	--mn;
#endif//MP_DEBUG
#ifndef MP_SINGLE_THREAD_MOD
	mp_enter_unused_lock(mp);
#endif//MP_SINGLE_THREAD_MOD
	mn->tailq_next = mp->unused_node[mn->pool_idx];
	mp->unused_node[mn->pool_idx] = mn;
#ifndef MP_SINGLE_THREAD_MOD
	mp_leave_unused_lock(mp);
#endif//MP_SINGLE_THREAD_MOD
}
