#include "memory_node.h"

uint64_t get_thread_id() {
#ifdef _WIN32
	return GetCurrentThreadId();
#else
	//��unixΪ��׼����ֵ64λ
	return pthread_self();
#endif//_WIN32
}

/*
	����num�� (1 << left) <= num < (1 << right) ��Χ��Ӧ�ô��� [ (1 << ret_var), (1 << ret_var) ) ��ֵ��
	num > (1 << right) ret_var = -1	//������Ҫ���ж�
	num < (1 << left) ret_var = left
	return value:
	-1: ������ָ���ƶ�λ�������ֵ
	0~31: num ���� [ (1 << ret_var - 1), (1 << ret_var) ) ��ֵ��
*/
int bit_range(uint8_t left, uint8_t right, uint32_t num) {
	char mid = left + ((right - left) / 2) + ((right - left) & 1);

	if (num > (1 << right)) {
		return -1;
	}
	else if (num < (1 << left)) {
		return left;
	}

	/*if (((1 << mid) <= num && num < (1 << (mid + 1))) || mid == right) {
		return mid;
	}*/
	if ((1 << mid) <= num && num < (1 << (mid + 1))) {
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
	bit_range(left, right, num);
}

int bit_range2(uint8_t left, uint8_t right, uint32_t num) {
	if (num > (1 << right)) {
		return -1;
	}
	else if (num < (1 << left)) {
		return left;
	}

	char mid;
bit_range2_s:
	mid = left + ((right - left) / 2) + ((right - left) & 1);
	/*if (((1 << mid) <= num && num < (1 << (mid + 1))) || mid == right) {
		return mid;
	}*/
	if ((1 << mid) <= num && num < (1 << (mid + 1))) {
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

void insert_tailq(memory_node_t** head, memory_node_t* mn) {
	mn->tailq_next = *head;
	if (*head) {
		(*head)->tailq_prev = mn;
	}
	*head = mn;
	mn->tailq_prev = head;
}

void remove_tailq(memory_node_t* mn) {
	*(mn->tailq_prev) = mn->tailq_next;
	if (mn->tailq_next) {
		mn->tailq_next->tailq_prev = mn->tailq_prev;
	}
}

memory_node_t* fmt_memory_node(char* pool, uint32_t all_size, uint64_t thread_id) {
	memory_node_t* head = pool;
	head->thread_id = thread_id;
	head->size = all_size;
	return head;
}

memory_node_t* cutting_tail(memory_node_t* head, uint32_t size) {
	int lave = head->size - sizeof(memory_node_t) - size;
	if (lave < 0) {
		return 0;
	}
	head->size = lave;

	memory_node_t* tail_node = ((char*)(head + 1)) + head->size;
	tail_node->size = size;
	tail_node->thread_id = head->thread_id;
	return tail_node;
	//return fmt_memory_node(((char*)(head + 1)) + head->size, size, head->thread_id);
}