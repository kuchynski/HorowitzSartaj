//
// kuchynskiandrei@gmail.com
// 2020
//

#include <time.h>
#include "Horowitz_Sartaj.h"

#define TOTAL_NUMBERS		48
#define INCLUDED_NUMBERS	40

void probabilistic_method(uint64_t a, int n, uint64_t *m);

int main(void)
{
	// --------------------------- Initialization -------------------------------------
	
    uint64_t amount = 0;
	int n;
	int included = 0;
    uint64_t coins[TOTAL_NUMBERS];
	time_t time_begin, time_end; 
	
	for(n = 0; n < TOTAL_NUMBERS; n ++) {
		coins[n] = rand();
		if(included++ < INCLUDED_NUMBERS) {
			amount += coins[n];
		}
	}

	printf("\nValue = %lld\n", amount);
	printf("%d numbers\n\n", TOTAL_NUMBERS);

	printf("------------ Horowitz_Sartaj -------------------------\n");
	time_begin = time(0);
	HorowitzSartaj(amount, TOTAL_NUMBERS, coins);
	time_end = time(0);
	printf("%d seconds\n", (int)(time_end - time_begin));

	printf("------------ Horowitz_Sartaj 3 -----------------------\n");
	time_begin = time(0);
	HorowitzSartaj3(amount, TOTAL_NUMBERS, coins);
	time_end = time(0);
	printf("%d seconds\n", (int)(time_end - time_begin));

	printf("------------ ss method -----------------------\n");
	time_begin = time(0);
	probabilistic_method(amount, TOTAL_NUMBERS, coins);
	time_end = time(0);
	printf("%d seconds\n", (int)(time_end - time_begin));
	return 0;
}

void probabilistic_method(uint64_t a, int n, uint64_t *m)
{
    uint64_t amount_full = 0;
	//const int max_attempts = 1000000000;
	time_t time_begin, time_end;
	uint64_t step;
	time_begin = time(0);
	for(int j = 0; j < n; j ++) {
		amount_full += m[j];
	}
	double coof = (double)a / (double)amount_full;
	uint32_t threshold = (uint32_t)(((double)RAND_MAX) * (coof+0.079));
	printf("\nValue = %lld %f, %d %d\n", amount_full, coof, threshold, RAND_MAX);
	amount_full = 0;
	for(step = 1; /* step < max_attempts*/; step++) {
		uint64_t amount = 0;
		for(int j = 0; j < n; j ++) {
			uint32_t r = rand();
			if(r < threshold) {
				amount += m[j];
				if(amount == a) {
					break;
				}
				if(amount > a)
					break;
			}
		}
		amount_full += amount;
		if(amount == a) {
			printf("2-----------------!!! %lld == %lld, %lld steps\n", amount, a, step);
			time_end = time(0);
			printf("%d seconds\n", (int)(time_end - time_begin));
			break;
		}
	}
	printf("middle \n%lld != \n%lld\n", amount_full/step, a);
}
