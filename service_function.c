//
// kuchynskiandrei@gmail.com
// 2020
//

#include "service_function.h"

void Sort(uint64_t *M, uint64_t *M_tmp, int start, int end)
{
	const int mid = (start + end) / 2;
	int i_left = start;
	int i_right = mid + 1;
	int i_tmp = i_left;
	int i;

	if(start == end)
		return;

	Sort(M, M_tmp, start, mid);
	Sort(M, M_tmp, mid+1, end);

	while((i_left <= mid) & (i_right <= end)) {
		if(M[i_left] <= M[i_right])
			M_tmp[i_tmp++] = M[i_left++];
		else
			M_tmp[i_tmp++] = M[i_right++];
	}

	for(i = i_right; i <= end; i++)
		M_tmp[i_tmp++] = M[i];
	for(i = i_left; i <= mid; i++)
		M_tmp[i_tmp++] = M[i];
	for(i = start; i <= end; i++)
		M[i] = M_tmp[i];
}
