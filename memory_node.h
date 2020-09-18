#ifndef _MEMORY_NODE_H_
#define _MEMORY_NODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct memory_node {
	struct memory_node*	tailq_next;
	uint32_t size;						//��ǰ�ڵ㳤��
	uint32_t use_size;					//��ǰ�ڵ�����ĳ��ȣ���ֵΪ�������reallocʱmemcpy������Ч��
	uint32_t pool_idx;					//�����ڴ�صķ�Χ (1 << pool_idx)������free����
}memory_node_t;

int bit_range2(uint8_t left, uint8_t right, uint32_t num);

/*pool_idx Ϊ������memory_pool::unused_node���б�����,����ָ������Ϊ0*/
memory_node_t* fmt_memory_node(char* pool, uint32_t all_size);

memory_node_t* cutting_tail(memory_node_t* head, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif//_MEMORY_NODE_H_