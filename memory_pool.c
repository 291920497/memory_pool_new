//#include "memory_pool.h"
#include "memory_pool.h"

#include <stdlib.h>

#include <assert.h>

//tailq critical section

static void mp_enter_unused_lock(memory_pool_t* mp) {
#ifndef _WIN32
	pthread_spin_lock(&(mp->mp_unused_lock));
#else
	EnterCriticalSection(&(mp->mp_unused_lock));
#endif//_WIN32
}

static void mp_level_unused_lock(memory_pool_t* mp) {
#ifndef _WIN32
	pthread_spin_unlock(&(mp->mp_unused_lock));
#else
	LeaveCriticalSection(&(mp->mp_unused_lock));
#endif//_WIN32
}



memory_pool_t* mp_create(uint32_t init_size) {
	int err_code[2];
	memory_pool_t* mp = malloc(sizeof(memory_pool_t));
	char* pool = malloc(init_size + sizeof(memory_node_t));
	assert(mp);
	assert(pool);

	//init
	memset(mp, 0, sizeof(memory_pool_t));
	mp->thread_id = get_thread_id();

	memory_node_t* mn = fmt_memory_node(pool, init_size, mp->thread_id);
	insert_tailq(&(mp->unused_node[MP_UNUSED_END - MP_UNUSED_BEGIN]), mn);

#ifdef _WIN32
	InitializeCriticalSection(&(mp->mp_unused_lock));
	//InitializeCriticalSection(&(mp->mp_inused_lock));
#else
	err_code[0] = pthread_spin_init(&(mp->mp_unused_lock), PTHREAD_PROCESS_PRIVATE);
	//err_code[1] = pthread_spin_init(&(mp->mp_inused_lock), PTHREAD_PROCESS_PRIVATE);
	assert(err_code[0] == 0);
	//assert(err_code[1] == 0);
#endif//_WIN32

	return mp;
}

void mp_destroy(memory_pool_t* mp) {
	memory_node_t** walk = &(mp->unused_node[MP_UNUSED_END - MP_UNUSED_BEGIN]);
	while (*walk) {
		void* free_ptr = *walk;
		*walk = (*walk)->tailq_next;
		remove_tailq(free_ptr);
		free(free_ptr);
	}

#ifndef _WIN32
	pthread_spin_destroy(&(mp->mp_unused_lock));
#else
	DeleteCriticalSection(&(mp->mp_unused_lock));
#endif//_WIN32
	free(mp);
}

/*
	mp_malloc	debug版本将不会返回空指针，参数及系统函数调用错误将由assert中断
				这将带来额外的开销,若期望忽略这些错误并返回空指针，可将断言替换为判断后 mn = 0; goto mp_malloc_end;
*/
void* mp_malloc(memory_pool_t* mp, size_t size) {
	int bit_idx;
	memory_node_t* mn = 0;
	char* new_pool;
	
	//返回位运算的右边界
	bit_idx = bit_range2(MP_UNUSED_BEGIN, MP_UNUSED_END, size);
	assert(bit_idx != -1);
	bit_idx -= MP_UNUSED_BEGIN;

	mp_enter_unused_lock(mp);
	//获取可完全容纳申请大小的内存块
	if (mn = mp->unused_node[bit_idx]) {
		remove_tailq(mn);
		goto mp_malloc_end;
	}

mp_malloc_try_agin:
	mn = cutting_tail(mp->unused_node[MP_UNUSED_POOL_SIZE - 1], bit_idx == 0 ? MP_SMALL_BLOCK : size);
	if (mn) {
		mn->pool_idx = bit_idx ? bit_idx - 1 : 0;
		goto mp_malloc_end;
	}
	//若申请的大小超过了默认值，则改变申请大小且预留一个头节点
	uint32_t new_size = MP_DEFAULT_ALLOC_SIZE + sizeof(memory_node_t) * 2;
	if (size > MP_DEFAULT_ALLOC_SIZE) {
		new_size = size + sizeof(memory_node_t) * 2;
	}
	new_pool = malloc(new_size);
	assert(new_pool);
	mn = fmt_memory_node(new_pool, new_size - sizeof(memory_node_t), mp->thread_id);
	insert_tailq(&(mp->unused_node[MP_UNUSED_POOL_SIZE - 1]), mn);
	goto mp_malloc_try_agin;

mp_malloc_end:
	mp_level_unused_lock(mp);
	return ++mn;
	//return mn;
}

void mp_free(memory_pool_t* mp, void* free_ptr) {
	memory_node_t* mn = free_ptr;
	--mn;
	mp_enter_unused_lock(mp);
	insert_tailq(&(mp->unused_node[mn->pool_idx]), mn);
	mp_level_unused_lock(mp);
}
