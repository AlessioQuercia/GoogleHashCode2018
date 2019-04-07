#include "choice_criteria.h"

/////////////////////////////////////////////////// CHOICE CRITERIA VERSION 2 ///////////////////////////////////////////////////

// Choice criterion to store the best ride in BEST: max_reward
void max_r(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int idr, int t)
{
	int wt = wait_time(SMP, DIST_S, idv, idr, t);

	int required_time = t + DIST_S[idv][idr] + wt + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((REW[idv][idr] > BEST[idv][0].val))											// first criterion: max_reward
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		mixtriple mc = { .val = REW[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}

// Choice criterion to store the best ride in BEST: min_required_time --> max_reward
void min_rt_max_r(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((required_time < BEST[idv][0].t) ||											// first criterion: min_required_time
		(required_time == BEST[idv][0].t) && (REW[idv][idr].val > BEST[idv][0].val)		// second criterion: max_reward
		)											
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		mixtriple mc = { .val = REW[idv][idr].val,.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> min_required_time
void min_wt_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)											// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time < BEST[idv][0].t))		// second criterion: minimum required_time
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		mixtriple mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> max_reward --> min_required_time
void min_wt_max_r_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ( ((WAIT[idv][idr] < BEST[idv][0].val)																									// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val)											// second criterion: maximum reward
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val == REW[idv][BEST[idv][0].idr].val && required_time < BEST[idv][0].t))		// third criterion: minimum required_time
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T )
	{
		mixtriple mc = { .val = WAIT[idv][idr], .idr = idr , .t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time
void min_wt_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)															// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt))								// second criterion: maximum ratio reward/required_time
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T )
	{
	mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

	BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time --> min_required_time
void min_wt_max_ratio_min_rt(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] +  SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ( ((WAIT[idv][idr] < BEST[idv][0].val)																		// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt)											// second criterion: maximum ratio reward/required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio == BEST[idv][0].rt && required_time < BEST[idv][0].t))		// third criterion: minimum required_time
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T )
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time, .rt = ratio };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time --> max_reward
void min_wt_max_ratio_max_r(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																							// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio > BEST[idv][0].rt)																// second criterion: maximum ratio reward/required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && ratio == BEST[idv][0].rt && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val))		// third criterion: maximum reward
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T )
	{
	mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

	BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> max_reward --> max_ratio_reward/required_time == min_wait_time --> max_reward --> min_required_time
void min_wt_max_r_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																							// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val)									// second criterion: maximum reward
		|| (WAIT[idv][idr] == BEST[idv][0].val && REW[idv][idr].val == REW[idv][BEST[idv][0].idr].val && ratio > BEST[idv][0].rt))		// third criterion: maximum ratio reward/required_time
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> min_required_time --> max_ratio_reward/required_time == min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	float ratio = (float)(REW[idv][idr].val) / (float)(required_time);

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, DIST_S[idv][idr] + SMP->rides[idr].dd);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (((WAIT[idv][idr] < BEST[idv][0].val)																							// first criterion: minimum wait_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time < BEST[idv][0].t)														// second criterion: minimum required_time
		|| (WAIT[idv][idr] == BEST[idv][0].val && required_time == BEST[idv][0].t && ratio > BEST[idv][0].rt))							// third criterion: maximum ratio reward/required_time
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		mixquad mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time,.rt = ratio };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_r(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int idr, int t)
{
	int wt = wait_time(SMP, DIST_S, idv, idr, t);

	int required_time = t + DIST_S[idv][idr] + wt + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, required_time);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ((
		(wt < BEST[idv][0].val)	||																								// first criterion: minimum wait_time
		(wt == BEST[idv][0].val && required_time < BEST[idv][0].t) ||															// second criterion: minimum required_time
		(wt == BEST[idv][0].val && required_time == BEST[idv][0].t && REW[idv][idr] > REW[idv][BEST[idv][0].idr])		// third criterion: maximum reward
		) && required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{

		mixtriple mc = { .val = wt,.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: zero_wait_time --> min_required_time --> max_reward
void zero_wt_min_rt_max_r(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, required_time);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if ( WAIT[idv][idr] == 0 && (																	// first criterion: zero wait_time
		(required_time < BEST[idv][0].t) ||															// second criterion: minimum required_time
		(required_time == BEST[idv][0].t && REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val)		// third criterion: maximum reward
		) && required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{

		mixtriple mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: zero_wait_time --> max_reward --> min_required_time 
void zero_wt_max_r_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t)
{
	int required_time = t + DIST_S[idv][idr] + WAIT[idv][idr] + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, required_time);

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (WAIT[idv][idr] == 0 && (																		// first criterion: zero wait_time
		(REW[idv][idr].val > REW[idv][BEST[idv][0].idr].val) ||											// second criterion: maximum reward
		(REW[idv][idr].val == REW[idv][BEST[idv][0].idr].val && required_time < BEST[idv][0].t)			// third criterion: minimum required_time
		) && required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{

		mixtriple mc = { .val = WAIT[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}


// Choice criterion to store the best ride in BEST: max_utility --> min_required_time
void max_u_min_rt(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int idr, int t)
{
	int wt = wait_time(SMP, DIST_S, idv, idr, t);
	int required_time = t + DIST_S[idv][idr] + wt + SMP->rides[idr].dd;

	//printf("%d, %f, %d, %d\n", WAIT[idv][idr], BEST[idv][0].val, t, required_time);

	//float avg_d = avg_distance(SMP, SMP->rides[idr].x, SMP->rides[idr].y);

	int utility = REW[idv][idr] - required_time; // -(int)avg_d;

	// If the wait_time of assigning ride r to vehicle v is smaller than the one in the current slot, update it with the smaller one
	if (( 
			(utility > BEST[idv][0].val) ||										// First criterion: maximum utility
			(utility == BEST[idv][0].val && required_time < BEST[idv][0].t)		// Second criterion: minimum required_time
		)
		&& required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{

		mixtriple mc = { .val = utility,.idr = idr ,.t = required_time };

		BEST[idv][0] = mc;
	}
}



// Choice criterion to store the best K rides in BEST: max_utility --> min_required_time
void max_u_min_rt_K(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int idr, int t, int K)
{
	if (SMP->rides[idr].done == 1)
		return;

	int wt = wait_time(SMP, DIST_S, idv, idr, t);

	int required_time = t + DIST_S[idv][idr] + wt + SMP->rides[idr].dd;

	if (required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		int utility = REW[idv][idr] - required_time;

		int j = K-1;
		while (j >= 0 && ( 
			(utility > BEST[idv][j].val) ||										// First criterion: maximum utility
			(utility == BEST[idv][j].val && required_time < BEST[idv][j].t)		// Second criterion: minimum required_time
			))
		{
			BEST[idv][j + 1] = BEST[idv][j];

			j--;
		}

		mixtriple mc = { .val = utility,.idr = idr ,.t = required_time };

		BEST[idv][j + 1] = mc;

		//printf("%d %d %f\n", j, utility, BEST[idv][0].val);
	}
}

// Choice criterion to store the best ride in BEST: min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_r_K(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int idr, int t, int K)
{
	if (SMP->rides[idr].done == 1)
		return;

	int wt = wait_time(SMP, DIST_S, idv, idr, t);

	int required_time = t + DIST_S[idv][idr] + wt + SMP->rides[idr].dd;

	if (required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		int j = K - 1;

		while (j >= 0 && (
			(BEST[idv][j].val == -1 || wt < BEST[idv][j].val) ||															// first criterion: minimum wait_time
			(wt == BEST[idv][j].val && required_time < BEST[idv][j].t) ||													// second criterion: minimum required_time
			(wt == BEST[idv][j].val && required_time == BEST[idv][j].t && REW[idv][idr] > REW[idv][BEST[idv][j].idr])		// third criterion: maximum reward
			))
		{
			BEST[idv][j + 1] = BEST[idv][j];

			j--;
		}

		mixtriple mc = { .val = wt,.idr = idr ,.t = required_time };

		BEST[idv][j + 1] = mc;

		//printf("%d %d %f\n", j, utility, BEST[idv][0].val);
	}
}

// Choice criterion to store the best ride in BEST: max_reward
void max_r_K(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int idr, int t, int K)
{
	if (SMP->rides[idr].done == 1)
		return;

	int wt = wait_time(SMP, DIST_S, idv, idr, t);

	int required_time = t + DIST_S[idv][idr] + wt + SMP->rides[idr].dd;

	if (required_time <= SMP->rides[idr].f && required_time <= SMP->T)
	{
		int j = K - 1;
		//printf("%d vs %f\n", REW[idv][idr], BEST[idv][j].val);
		while (j >= 0 && (
			(REW[idv][idr] > BEST[idv][j].val)						// first criterion: max_reward
			))
		{
			BEST[idv][j + 1] = BEST[idv][j];

			j--;
		}

		mixtriple mc = { .val = REW[idv][idr],.idr = idr ,.t = required_time };

		BEST[idv][j + 1] = mc;

		//printf("%d %d %f\n", j, utility, BEST[idv][0].val);
	}
}



/////////////////////////////////////////////////// CHOICE CRITERIA VERSION 1 ///////////////////////////////////////////////////

// Stores the best rides according to the rewards
void store_best_rewards(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{
	// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
	if (REW[idv][idr].val > BEST[idv][0].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP->T)
	{
		//couple ctemp = BEST[idv][i];
		mixcouple mc = { .val = REW[idv][idr].val,.idr = REW[idv][idr].idr };

		BEST[idv][0] = mc;

		//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
	}
}



// Stores the best rides according to the ratio reward/time_to_wait
void store_best_rewards_time_to_wait(sample *SMP, int **DIST_S, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{
	for (int i = 0; i < NB; i++)
	{
		// If the current slot is empty, assign the ride to it
		if (BEST[idv][i].idr == -1)
		{

			BEST[idv][i] = REW[idv][idr];
			slots--;
			break;
		}
		// If the current slot is not empty
		else
		{
			// If there is at least another empty slot, and the current slot is not empty, skip to the next slot
			if (slots > 0)
			{
				continue;
			}
			// If there are no more empty slots in BEST[idv]
			else
			{
				// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
				if (REW[idv][idr].val > BEST[idv][i].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP->T)
				{
					//couple ctemp = BEST[idv][i];

					BEST[idv][i] = REW[idv][idr];

					//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
				}
			}
		}
	}
}


// Stores the best rides according to the ratio reward/required_time
void store_best_rewards_required_time(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{

	float wait_time = max(0, SMP->rides[idr].s - (t + DIST_S[idv][idr]));
	float total_time = DIST_S[idv][idr] + SMP->rides[idr].dd + wait_time;
	mixcouple mc = { .val = (float)REW[idv][idr].val / total_time,.idr = idr };

	// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
	if (mc.val > BEST[idv][0].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP->T)
	{
		//couple ctemp = BEST[idv][i];

		BEST[idv][0] = mc;

		//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
	}
}



// Stores the best rides according to the distances from the vehicles to the rides
void store_best_distances(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{
	// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
	if (DIST_S[idv][idr] < BEST[idv][0].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP->T)
	{
		//couple ctemp = BEST[idv][i];

		mixcouple c = { .val = DIST_S[idv][idr],.idr = idr };

		BEST[idv][0] = c;

		//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
	}
}


// Stores the best rides according to the distances from the vehicles to the rides
void store_best_total_distances(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{
	// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
	if (DIST_S[idv][idr] + SMP->rides[idr].dd < BEST[idv][0].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP->T)
	{
		//couple ctemp = BEST[idv][i];

		mixcouple c = { .val = DIST_S[idv][idr] + SMP->rides[idr].dd,.idr = idr };

		BEST[idv][0] = c;

		//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
	}
}