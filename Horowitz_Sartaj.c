//
// kuchynskiandrei@gmail.com
// 2020
//

#include "Horowitz_Sartaj.h"
#include "service_function.h"
#include "bmemory.h"
#include <string.h>

static uint64_t *mas;
static int count_iterations;

void InitRecurs(uint64_t result, int n, uint64_t *M, int *count, int border)
{
	count_iterations ++;
	if(n < border) {
		InitRecurs(result, n + 1, M, count, border);
		InitRecurs(result + mas[n], n + 1, M, count, border);
	}
	else {
		M[*count] = result;
		*count += 1;
		M[*count] = result + mas[n];
		*count += 1;
	}
}

void InitRecurs3(uint64_t result, int n, struct BMemory *b_memory, int border)
{
	count_iterations ++;
	if(n < border) {
		InitRecurs3(result, n + 1, b_memory, border);
		InitRecurs3(result + mas[n], n + 1, b_memory, border);
	}
	else {
		if(result)
			BMemoryAddPointer(b_memory, result, 0, (void*)2);
		BMemoryAddPointer(b_memory, result + mas[n], 0, (void*)2);
	}
}

void HorowitzSartaj(uint64_t a, int n, uint64_t *m)
{
	uint64_t *M0;
	uint64_t *M1;
	uint64_t *M_tmp;
	const int border0 = n / 2;
	const int border1 = n;
	int sizeM0 = 1 << border0;
	int sizeM1 = 1 << (border1 - border0);
	int i, j;
	int count_solutions = 0;
	uint64_t memory_used;

	mas = m;
	count_iterations = 0;

	M0 = (uint64_t*)malloc(sizeof(uint64_t) * sizeM0);
	if(M0 == NULL) {
		printf("There is no enough space into heap 1\n");
		return;
	}
	memory_used = sizeof(uint64_t) * sizeM0;
	M1 = (uint64_t*)malloc(sizeof(uint64_t) * sizeM1);
	if(M1 == NULL) {
		printf("There is no enough space into heap 2\n");
		free(M0);
		return;
	}
	memory_used += sizeof(uint64_t) * sizeM1;
	M_tmp = (uint64_t*)malloc(sizeof(uint64_t) * sizeM1);
	if(M_tmp == NULL) {
		printf("There is no enough space into heap 3\n");
		free(M0);
		free(M1);
		return;
	}
	memory_used += sizeof(uint64_t) * sizeM1;

	sizeM0 = 0;
	InitRecurs(0, 0, M0, &sizeM0, border0 - 1);
	sizeM1 = 0;
	InitRecurs(0, border0, M1, &sizeM1, border1 - 1);

	Sort(M0, M_tmp, 0, sizeM0-1);
	Sort(M1, M_tmp, 0, sizeM1-1);
	free(M_tmp);

	j = sizeM1 - 1;
	for(i = 0; i < sizeM0; i ++) {
		const uint64_t m0 = M0[i];
		if(m0 > a)
			break;
		else if(m0 == a) {
			count_solutions++;
			printf("solution1 %d\n", count_solutions);
			continue;
		}
		const uint64_t rest = a - m0;
		for(; j > 0; j --) {
			const uint64_t m1 = M1[j];
			count_iterations ++;
			if(m1 < rest)
				break;
			else if(m1 == rest) {
				count_solutions++;
				//printf("solution2 %d: %lld + %lld\n", count_solutions, m0, m1);
				continue;
			}
		}
	}
	
	free(M0);
	free(M1);
	printf("%d steps, %d solutions\n", count_iterations, count_solutions);
	printf("%lu memory used\n", memory_used);
}

void HorowitzSartaj3(uint64_t a, int n, uint64_t *m)
{
	uint64_t *M0;
	uint64_t *M1;
	uint64_t *M_tmp;
	const int border0 = n / 3;
	const int border1 = border0 * 2;
	int sizeM0 = 1 << border0;
	int sizeM1 = 1 << (border1 - border0);
	int i, j;
	int j_max;
	int count_solutions = 0;
	uint64_t memory_used;
	uint64_t *m_sort;
	struct BMemory b_memory;
	uint64_t max2 = 0, min2 = 0;

	count_iterations = 0;

	m_sort = (uint64_t*)malloc(sizeof(uint64_t) * n);
	if(m_sort == NULL) {
		printf("There is no enough space\n");
		return;
	}
	mas = m_sort;
	
	// Step 1 ----- Sort input data ---------------
	for(i = 0; i < n; i ++)
		m_sort[i] = m[i];
	for(i = 0; i < n; i ++) {
		for(j = i+1; j < n; j ++) {
			if(m_sort[i] < m_sort[j]) {
				uint64_t tmp = m_sort[i];
				m_sort[i] = m_sort[j];
				m_sort[j] = tmp;
			}
		}
	}	
	
	// Step 2 ----- Create the first data block ---------------
	M0 = (uint64_t*)malloc(sizeof(uint64_t) * sizeM0);
	if(M0 == NULL) {
		printf("There is no enough space into heap 1\n");
		return;
	}
	memory_used = sizeof(uint64_t) * sizeM0;
	// Step 3 ----- Create the second data block ---------------
	M1 = (uint64_t*)malloc(sizeof(uint64_t) * sizeM1);
	if(M1 == NULL) {
		printf("There is no enough space into heap 2\n");
		free(M0);
		return;
	}
	memory_used += sizeof(uint64_t) * sizeM1;
	M_tmp = (uint64_t*)malloc(sizeof(uint64_t) * sizeM1);
	if(M_tmp == NULL) {
		printf("There is no enough space into heap 3\n");
		free(M0);
		free(M1);
		return;
	}
	memory_used += sizeof(uint64_t) * sizeM1;

	sizeM0 = 0;
	InitRecurs(0, 0, M0, &sizeM0, border0 - 1);
	sizeM1 = 0;
	InitRecurs(0, border0, M1, &sizeM1, border1 - 1);

	// Step 4 ----- Sort first and second blocks ---------------
	Sort(M0, M_tmp, 0, sizeM0-1);
	Sort(M1, M_tmp, 0, sizeM1-1);
	free(M_tmp);

	// Step 5 ----- Create the third data block ---------------
	memset(&b_memory, 0, sizeof(struct BMemory));
	InitRecurs3(0, border1, &b_memory, n - 1);
	min2 = m_sort[n-1];
	for(i = border1, max2 = 0; i < n; i ++)
		max2 += m_sort[i];


	// Step 6 -----  ---------------
	j_max = sizeM1 - 1;
	for(i = 0; i < sizeM0; i ++) {
		const uint64_t m0 = M0[i];
		const uint64_t rest = a - m0;

		if(m0 > a)
			break;
		else if(rest == 0) {
			count_solutions++;
			//printf("solution1 %d\n", count_solutions);
			continue;
		}
		else {
			uint64_t res = (uint64_t)BMemoryGetPointer(&b_memory, rest, 0);
			if(res) {
				count_solutions += (res-1);
				//printf("solution2 %d\n", count_solutions);
			}
		}
			
		
		for(j = j_max; j > 0; j --) {
			const uint64_t m1 = M1[j];
			const uint64_t rest1 = rest - m1;
			count_iterations ++;
			
			if(m1 > rest) {
				j_max = j - 1;
//printf("-j_max %lld\n", j_max);
				continue;
			}
			else if(rest1 == 0) {
				count_solutions++;
				//printf("solution3 %d\n", count_solutions);
				continue;
			}
			else { // m1 < rest
				if(rest1 > max2) {
//printf("rest1 > max2 %lld\n", max2);
					break;
				}
				uint64_t res = (uint64_t)BMemoryGetPointer(&b_memory, rest1, 0);
				if(res) {
					count_solutions += (res-1);
				}
			}
		}
	}
	
	free(m_sort);
	free(M0);
	free(M1);
	memory_used += BMemoryRemoveAll(&b_memory, 0);

	printf("%d steps, %d solutions\n", count_iterations, count_solutions);
	printf("%lu memory used\n", memory_used);
}

