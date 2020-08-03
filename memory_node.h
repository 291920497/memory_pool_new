#ifndef _MEMORY_NODE_H_
#define _MEMORY_NODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct memory_node {
	struct memory_node*	tailq_next;
	struct memory_node** tailq_prev;	//用于管理节点，且不能修改在结构体的位置
	uint64_t thread_id;					//当前节点所属线程
	uint32_t size;						//当前节点长度
	uint32_t pool_idx;					//所在内存池的范围 (1 << pool_idx)，减少free运算
}memory_node_t;

uint64_t get_thread_id();

int bit_range(uint8_t left, uint8_t right, uint32_t num);

int bit_range2(uint8_t left, uint8_t right, uint32_t num);

void insert_tailq(memory_node_t** head, memory_node_t* mp);

void remove_tailq(memory_node_t* mp);

/*pool_idx 为其所在memory_pool::unused_node的列表索引,若不指定则设为0*/
memory_node_t* fmt_memory_node(char* pool, uint32_t all_size, uint64_t thread_id);

memory_node_t* cutting_tail(memory_node_t* head, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif//_MEMORY_NODE_H_