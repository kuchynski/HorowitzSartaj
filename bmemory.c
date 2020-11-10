//
// kuchynskiandrei@gmail.com
// 2020
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmemory.h"

static uint64_t memory_used = 0;

void BMemoryAddPointer(struct BMemory *b_memory, uint64_t key, int deep, void *pointer)
{
	const uint64_t symbol = key & KEY_SYMBOL_MASK;

	if(deep >= KEY_DEEPEST) {
		if(b_memory->pointer[symbol] == NULL)
			b_memory->pointer[symbol] = pointer;	// set successfully
		else {
			b_memory->pointer[symbol] += 1;	// set successfully
		}
	}
	else if(b_memory->alone_pointer == NULL) {
		b_memory->alone_key = key;
		b_memory->alone_pointer = pointer;
	}
	else {
		void *next = b_memory->pointer[symbol];
		if(next == NULL) {
			next = malloc(sizeof(struct BMemory));
			memory_used += sizeof(struct BMemory);
			if(next == NULL)
				return;
			memset(next, 0, sizeof(struct BMemory));
			b_memory->pointer[symbol] = next;
		}
		if(b_memory->alone_pointer != (void*)1) { // there is at least one element here
			void *pointer_tmp = b_memory->alone_pointer;
			b_memory->alone_pointer = (void*)1;
			BMemoryAddPointer(b_memory, b_memory->alone_key, deep, pointer_tmp);
		}
		BMemoryAddPointer((struct BMemory*)next, key >> KEY_SYMBOL_WIDTH, deep+1, pointer);
	}
}

void BMemoryRemovePointer(struct BMemory *b_memory, const uint64_t key, int deep)
{
	const uint64_t symbol = key & KEY_SYMBOL_MASK;
	if(deep >= KEY_DEEPEST) {
		b_memory->pointer[symbol] = NULL;
	}
	else if(b_memory->alone_pointer == NULL) {
	}
	else if(b_memory->alone_pointer != (void*)1) {
		if(b_memory->alone_key == key)
			b_memory->alone_pointer = NULL;
	}
	else { //b_memory->alone_pointer == (void*)1
		void *next = b_memory->pointer[symbol];
		if(next) {
			BMemoryRemovePointer((struct BMemory*)next, key >> KEY_SYMBOL_WIDTH, deep+1);
		}
	}
}

uint64_t BMemoryRemoveAll(struct BMemory *b_memory, int deep)
{
	if((deep < KEY_DEEPEST) && (b_memory->alone_pointer == (void*)1)) {
		uint64_t symbol;
		for(symbol = 1; symbol <= KEY_SYMBOL_MASK; symbol++) {
			void *next = b_memory->pointer[symbol];
			if(next) {
				BMemoryRemoveAll((struct BMemory*)next, deep+1);
				free(next);
			}
		}
	}
	return memory_used;
}

void* BMemoryGetPointer(struct BMemory *b_memory, const uint64_t key, int deep)
{
	const uint64_t symbol = key & KEY_SYMBOL_MASK;
	void *ret = NULL;
	if(deep >= KEY_DEEPEST) {
		ret = b_memory->pointer[symbol];
	}
	else if(b_memory->alone_pointer == NULL) {
		printf("we can't be here\n");
	}
	else if(b_memory->alone_pointer != (void*)1) {
		if(b_memory->alone_key == key) {
			ret = b_memory->alone_pointer;
		}
	}
	else { //b_memory->alone_pointer == (void*)1
		void *next = b_memory->pointer[symbol];
		if(next) {
			ret = BMemoryGetPointer((struct BMemory*)next, key >> KEY_SYMBOL_WIDTH, deep+1);
		}
	}
	return ret;
}
