#include "choice_criteria.h"

// Choice criterium to store the best ride in BEST
void min_wait_time(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ( ((WAIT[idv][idr] < BEST[idv][0].val) || (WAIT[idv][idr] == BEST[idv][0].val && required_time < BEST[idv][0].val)) 
		&& required_time <= SMP->T)
	{
		mixtriple mc = { .val = WAIT[idv][idr], .idr = idr , .t = required_time};

		BEST[idv][0] = mc;
	}
}