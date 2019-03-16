#include "choice_criteria.h"

// Choice criterium to store the best ride in BEST
void min_wait_time(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ( ((WAIT[idv][idr] < BEST[idv][0].val)																									// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val)											// second criterium: maximum reward
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val == REW[idv][BEST[idv][0].idr].val && required_time < BEST[idv][0].t))		// third criterium: minimum required_time
		&& required_time <= SMP->T )
	{
		mixtriple mc = { .val = WAIT[idv][idr], .idr = idr , .t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST
void min_wait_time_ratio_reward_required_time(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (  ((WAIT[idv][idr] < BEST[idv][0].val)															// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt))								// second criterium: maximum ratio reward/required_time
		&& required_time <= SMP->T)
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time, .rt = ratio };

		BEST[idv][0] = mc;
	}
}