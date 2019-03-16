#include "choice_criteria.h"


// Choice criterium to store the best ride in BEST: min_wait_time --> min_required_time
void min_wt_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)											// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time < BEST[idv][0].t))		// second criterium: minimum required_time
		&& required_time <= SMP->T)
	{
		mixtriple mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST: min_wait_time --> max_reward --> min_required_time
void min_wt_max_r_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

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


// Choice criterium to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time
void min_wt_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)															// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt))								// second criterium: maximum ratio reward/required_time
		&& required_time <= SMP->T )
	{
	mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

	BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time --> min_required_time
void min_wt_max_ratio_min_rt(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] +  SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ( ((WAIT[idv][idr] < BEST[idv][0].val)																		// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt)											// second criterium: maximum ratio reward/required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio == BEST[idv][0].rt && required_time < BEST[idv][0].t))		// third criterium: minimum required_time
		&& required_time <= SMP->T )
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time, .rt = ratio };

		BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time --> max_reward
void min_wt_max_ratio_max_r(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																							// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt)																// second criterium: maximum ratio reward/required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio == BEST[idv][0].rt && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val))		// third criterium: maximum reward
		&& required_time <= SMP->T )
	{
	mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

	BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST: min_wait_time --> max_reward --> max_ratio_reward/required_time == min_wait_time --> max_reward --> min_required_time
void min_wt_max_r_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																							// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val)									// second criterium: maximum reward
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val == REW[idv][BEST[idv][0].idr].val && ratio > BEST[idv][0].rt))		// third criterium: maximum ratio reward/required_time
		&& required_time <= SMP->T)
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

		BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST: min_wait_time --> min_required_time --> max_ratio_reward/required_time == min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																							// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time < BEST[idv][0].t)														// second criterium: minimum required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time == BEST[idv][0].t && ratio > BEST[idv][0].rt))							// third criterium: maximum ratio reward/required_time
		&& required_time <= SMP->T)
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

		BEST[idv][0] = mc;
	}
}


// Choice criterium to store the best ride in BEST: min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_r(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																									// first criterium: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time < BEST[idv][0].t)																// second criterium: minimum required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time == BEST[idv][0].t && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val))		// third criterium: maximum reward
		&& required_time <= SMP->T)
	{
		mixtriple mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}