//
// kuchynskiandrei@gmail.com
// 2020
//

#include <time.h>
#include "Horowitz_Sartaj.h"

#define TOTAL_NUMBERS		48
#define INCLUDED_NUMBERS	40

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

	return 0;
}
