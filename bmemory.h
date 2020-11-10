//
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef H_BMEMORY
#define H_BMEMORY

#include <stdio.h>
#include <iostream> 

#define KEY_WIDTH			64
#define KEY_SYMBOL_WIDTH	4		// should be <= 16
#define KEY_SYMBOL_MASK		(0xFFFF >> (16-KEY_SYMBOL_WIDTH))
#define KEY_DEEPEST			(KEY_WIDTH / KEY_SYMBOL_WIDTH - ((KEY_WIDTH % KEY_SYMBOL_WIDTH)? 0 : 1))

#include <stdio.h>
#include <iostream> 
struct BMemory {
	void *pointer[KEY_SYMBOL_MASK+1];
	uint64_t alone_key;
	void *alone_pointer;
};

void BMemoryAddPointer(struct BMemory *b_memory, uint64_t key, int deep, void *pointer);
void BMemoryRemovePointer(struct BMemory *b_memory, const uint64_t key, int deep);
uint64_t BMemoryRemoveAll(struct BMemory *b_memory, int deep);
void* BMemoryGetPointer(struct BMemory *b_memory, const uint64_t key, int deep);

#endif