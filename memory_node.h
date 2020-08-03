#ifndef _MEMORY_NODE_H_
#define _MEMORY_NODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct memory_node {
	struct memory_node*	tailq_next;
	struct memory_node** tailq_prev;	//���ڹ���ڵ㣬�Ҳ����޸��ڽṹ���λ��
	uint64_t thread_id;					//��ǰ�ڵ������߳�
	uint32_t size;						//��ǰ�ڵ㳤��
	uint32_t pool_idx;					//�����ڴ�صķ�Χ (1 << pool_idx)������free����
}memory_node_t;

uint64_t get_thread_id();

int bit_range(uint8_t left, uint8_t right, uint32_t num);

int bit_range2(uint8_t left, uint8_t right, uint32_t num);

void insert_tailq(memory_node_t** head, memory_node_t* mp);

void remove_tailq(memory_node_t* mp);

/*pool_idx Ϊ������memory_pool::unused_node���б�����,����ָ������Ϊ0*/
memory_node_t* fmt_memory_node(char* pool, uint32_t all_size, uint64_t thread_id);

memory_node_t* cutting_tail(memory_node_t* head, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif//_MEMORY_NODE_H_