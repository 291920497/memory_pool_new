#include "memory_node.h"

/*
	计算num的值域, 值域与retval的关系: (1 << (retval - 1) , 1 << (retval)]
*/
int bit_range2(uint8_t left, uint8_t right, uint32_t num) {
	if (num > (1 << right)) {
		return -1;
	}
	else if (num <= (1 << left)) {
		return left;
	}

	char mid;
bit_range2_s:
	mid = left + ((right - left) / 2) + ((right - left) & 1);
	if ((1 << mid) < num && num <= (1 << (mid + 1))) {
		return mid + 1;
	}
	else if (mid == right) {
		return mid;
	}

	if ((1 << mid) < num) {
		left = mid;
	}
	else {
		right = mid;
	}
	goto bit_range2_s;
}


memory_node_t* fmt_memory_node(char* pool, uint32_t all_size) {
	memory_node_t* head = pool;
	head->size = all_size;
	return head;
}

/*
	剪掉尾巴并作为节点返回
*/
memory_node_t* cutting_tail(memory_node_t* head, uint32_t size) {
	int sub_size = sizeof(memory_node_t) + size;
	int lave = head->size - sub_size;
	if (lave < 0) {
		return 0;
	}
	head->size = lave;
	head->use_size += sub_size;

	memory_node_t* tail_node = ((char*)(head + 1)) + head->size;
	tail_node->size = size;
	tail_node->tailq_next = 0;
	return tail_node;
}