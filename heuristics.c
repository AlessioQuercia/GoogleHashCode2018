#include "heuristics.h"
#include "choice_criteria.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

// Returns the distance between two points (a,b) and (x,y)
int distance(int a, int b, int x, int y)
{
	return abs(a - x) + abs(b - y);
}


// Returns the reward given by assigning the ride r to the vehicle v at time t
int reward(vehicle v, ride r, int dist, int t, int T, int B)
{
	int rew = 0;

	int tds = t + dist;

	// If the vehicle will arrive in time (or earlier) at the starting point, starting from the time t, bonus B will be added
	if (tds <= r.s)
		rew += B;

	// If the vehicle will arrive at destination before the end T, points proportional to the distance (r.dd) will be added
	if (tds + r.dd < T)
		rew += r.dd;

	return rew;
}


// Returns the wait_time for the couple (vehicle, ride)
int wait_time(sample *SMP, int **DIST_S, int idv, int idr, int t)
{
	return max(0, SMP->rides[idr].s - (t + DIST_S[idv][idr]));	// max( 0, earliest_start - (current_t + distance_vechicle_ride) )
}



// Starts a simulation with wait times
void start_simulation_v2(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int NB, int *slots)
{
	printf("SIMULATION V2 STARTED\n");
	// For each vehicle
	for (int i = 0; i < SMP->F; i++)
	{
		int t = 0;	// Current vehicle step

		update_rides(SMP, DIST_S, REW, BEST, WAIT, i, t);	// Update vehicle possible rides

		//printf("VEHICLE %d:\n", i);

		while ((t += assign_ride(SMP, DIST_S, REW, BEST, WAIT, i, t)) <= SMP->T)	// Assign a new ride (if possible) and increment the current step by the required_time of the assigned ride
		{
			//printf("RIDE %d: %d\n", SMP->vehicles[i].ride.id, t);
			// Move to the assigned ride destination
			move(SMP, i);

			// Update vehicle possible rides
			update_rides(SMP, DIST_S, REW, BEST, WAIT, i, t);
		}

		//printf("TIME %d\n", t);
	}

	printf("SIMULATION V2 ENDED\n");
}

// Assigns a ride according to a choice_criterium and returns the time required to complete the ride
int assign_ride(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int t)
{
	int required_time = INT_MAX - t;

	// If the *best* ride (according to a choice_criterium) is not done yet, assign it
	if (SMP->rides[BEST[idv][0].idr].done == 0)
	{
		SMP->rides[BEST[idv][0].idr].done = 1;
		SMP->vehicles[idv].ride = SMP->rides[BEST[idv][0].idr];
		SMP->vehicles[idv].ds = DIST_S[idv][BEST[idv][0].idr];

		int wait_t = wait_time(SMP, DIST_S, idv, SMP->vehicles[idv].ride.id, t);	// max( 0, earliest_start - (current_t + distance_vechicle_ride) )

		required_time = SMP->vehicles[idv].ds + wait_t + SMP->vehicles[idv].ride.dd;	// distance_vechicle_ride + wait_time + distance_ride_destination
	}

	//printf("BEST IDR: %d\n", BEST[idv][0].idr);

	return required_time;
}

// Makes an entire ride
void move(sample *SMP, int idv)
{
	SMP->vehicles[idv].r = SMP->vehicles[idv].ride.a;
	SMP->vehicles[idv].c = SMP->vehicles[idv].ride.b;
	SMP->vehicles[idv].nrd++;
	SMP->vehicles[idv].idrd[SMP->vehicles[idv].ride.id] = 1;
}


void update_rides(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int t)
{
	// Re-initialize BEST
	mixtriple mc = { .val = INT_MAX,.idr = -1, .t=INT_MAX };

	BEST[idv][0] = mc;

	// For each ride
	for (int i = 0; i < SMP->N; i++)
	{
		if (SMP->rides[i].done == 1)
			continue;

		// Compute the distance from the vehicle current position to the considered ride
		DIST_S[idv][i] = distance(SMP->vehicles[idv].r, SMP->vehicles[idv].c, SMP->rides[i].a, SMP->rides[i].b);

		// Compute the wait_time for the considered ride
		WAIT[idv][i] = wait_time(SMP, DIST_S, idv, i, t);

		// Compute the reward for the considered ride
		int rwrd = reward(SMP->vehicles[idv], SMP->rides[i], DIST_S[idv][i], t, SMP->T, SMP->B);

		couple c = { .val = rwrd,.idr = SMP->rides[i].id };

		REW[idv][i] = c;

		// Choice criterium to store the best ride in BEST
		min_wait_time(SMP, DIST_S, REW, BEST, WAIT, idv, i, t);
	}

	//printf("%f, %d, %d\n", BEST[idv][0].val, BEST[idv][0].idr, BEST[idv][0].t);
}


// Initializes the required structures for v2
void initialize_structures(sample *SMP, int **DIST_S, mixtriple **BEST, couple **REW, int **WAIT, int t)
{
	printf("INITIALIZING STRUCTURES\n");
	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < SMP->N; j++)
		{
			DIST_S[i][j] = distance(SMP->vehicles[i].r, SMP->vehicles[i].c, SMP->rides[j].a, SMP->rides[j].b);	// Initialze distances from vehicles to rides

			WAIT[i][j] = wait_time(SMP, DIST_S, i, j, t);	// Initialize wait_times

			int rwrd = reward(SMP->vehicles[i], SMP->rides[j], DIST_S[i][j], t, SMP->T, SMP->B);

			couple c = { .val = rwrd,.idr = SMP->rides[j].id };

			REW[i][j] = c;	// Initialize rewards

			// Choice criterium to store the best ride in BEST
			min_wait_time(SMP, DIST_S, REW, BEST, WAIT, i, j, t);
		}
	}
}


void start_simulation(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, int NB, int *slots)
{
	printf("SIMULATION STARTED\n");

	int t = 0;

	while (t < SMP->T)
	{
		//printf("Time: %d\n", t);

		// Move each vehicle of 1 step
		move_vehicles(SMP, DIST_S, REW, BEST, NB, t, slots);

		t++;
	}

	printf("SIMULATION ENDED\n");

	//printf("Distances from vehicles to starting points\n");
	//for (int i = 0; i < SMP->F; i++)
	//{
	//	for (int j = 0; j < SMP->N; j++)
	//	{
	//		printf("%d ", DIST_S[i][j]);
	//	}
	//	printf("\n");
	//}

	//printf("Distances from starting points to destination points\n");
	//for (int i = 0; i < SMP->N; i++)
	//{
	//	printf("%d\n", SMP->rides[i].dd);
	//}

	//printf("Distances from vehicles to starting points\n");
	//for (int i = 0; i < SMP->F; i++)
	//{
	//	for (int j = 0; j < SMP->N; j++)
	//	{
	//		printf("(%d, %d) ", REW[i][j].idr, REW[i][j].rew);
	//	}
	//	printf("\n");
	//}

	//printf("Best couples\n");
	//for (int i = 0; i < SMP->F; i++)
	//{
	//	for (int j = 0; j < NB; j++)
	//	{
	//		printf("(%d, %d) ", BEST[i][j].idr, BEST[i][j].rew);
	//	}
	//	printf("\n");
	//}
}



void assign_single_ride(sample *SMP, int **DIST_S, vehicle *v, couple **REW, mixcouple **BEST, int NB, int t, int *slots)
{
	for (int i = 0; i < NB; i++)
	{
		if (SMP->rides[BEST[v->id][i].idr].done == 0)
		{
			//printf("RIDE %d \n", BEST[v->id][i].idr);

			SMP->rides[BEST[v->id][i].idr].done = 1;
			v->av = 1;
			v->ride = SMP->rides[BEST[v->id][i].idr];

			v->ds = DIST_S[v->id][BEST[v->id][i].idr];

			// Updates the distances in DIST_S, the rewards in REW to be assigned when the vehicle reaches its destination, and the best rides in BEST
			update_structures(SMP, DIST_S, REW, BEST, v, t, NB, slots);

			//printf("RIDE %d ASSIGNED AT VEHICLE %d.\n", BEST[v->id][i].idr, v->id);

			break;
		}
	}
}





void assign_single_ride_best_or_random(sample *SMP, int **DIST_S, vehicle *v, couple **REW, mixcouple **BEST, int NB, int t, int *slots)
{
	// Assign the best ride
	if (SMP->rides[BEST[v->id][0].idr].done == 0)
	{
		//printf("RIDE %d \n", BEST[v->id][i].idr);

		SMP->rides[BEST[v->id][0].idr].done = 1;
		v->av = 1;
		v->ride = SMP->rides[BEST[v->id][0].idr];

		v->ds = DIST_S[v->id][BEST[v->id][0].idr];

		// Updates the distances in DIST_S, the rewards in REW to be assigned when the vehicle reaches its destination, and the best rides in BEST
		update_structures(SMP, DIST_S, REW, BEST, v, t, NB, slots);

		//printf("RIDE %d ASSIGNED AT VEHICLE %d.\n", BEST[v->id][i].idr, v->id);
	}
	// Assign the first available ride (pseudo-random)
	else
	{
		for (int i = 0; i < SMP->N; i++)
		{
			if (SMP->rides[i].done == 0)
			{
				SMP->rides[i].done = 1;
				v->av = 1;
				v->ride = SMP->rides[i];

				v->ds = DIST_S[v->id][i];

				// Updates the distances in DIST_S, the rewards in REW to be assigned when the vehicle reaches its destination, and the best rides in BEST
				update_structures(SMP, DIST_S, REW, BEST, v, t, NB, slots);

				break;
			}
		}
	}
}



void assign_single_ride_update_best(sample *SMP, int **DIST_S, vehicle *v, couple **REW, mixcouple **BEST, int NB, int t, int *slots)
{
	// Assign the best ride
	if (SMP->rides[BEST[v->id][0].idr].done == 0)
	{
		//printf("RIDE %d \n", BEST[v->id][i].idr);

		SMP->rides[BEST[v->id][0].idr].done = 1;
		v->av = 1;
		v->ride = SMP->rides[BEST[v->id][0].idr];

		v->ds = DIST_S[v->id][BEST[v->id][0].idr];

		// Updates the distances in DIST_S, the rewards in REW to be assigned when the vehicle reaches its destination, and the best rides in BEST
		update_structures(SMP, DIST_S, REW, BEST, v, t, NB, slots);

		//printf("RIDE %d ASSIGNED AT VEHICLE %d.\n", BEST[v->id][i].idr, v->id);
	}
	// Assign the first available ride (pseudo-random)
	else
	{
		// Updates the distances in DIST_S, the rewards in REW to be assigned when the vehicle reaches its destination, and the best rides in BEST
		update_structures(SMP, DIST_S, REW, BEST, v, t, NB, slots);

		if (SMP->rides[BEST[v->id][0].idr].done == 0)
		{
			SMP->rides[BEST[v->id][0].idr].done = 1;
			v->av = 1;
			v->ride = SMP->rides[BEST[v->id][0].idr];

			v->ds = DIST_S[v->id][BEST[v->id][0].idr];

			// Updates the distances in DIST_S, the rewards in REW to be assigned when the vehicle reaches its destination, and the best rides in BEST
			update_structures(SMP, DIST_S, REW, BEST, v, t, NB, slots);
		}
	}
}



void assign_rides(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, int NB, int t, int *slots)
{
	printf("ASSIGNING RIDES\n");
	for (int i = 0; i < SMP->F; i++)
	{
		//printf("VEHICLE %d: ", i);

		assign_single_ride_best_or_random(SMP, DIST_S, &(SMP->vehicles[i]), REW, BEST, NB, t, slots);
	}
}



// Moves the vehicle v to the next step
void move_single_vehicle(sample *SMP, int **DIST_S, vehicle *v, couple **REW, mixcouple **BEST, int NB, int t, int *slots)
{
	// If it still has to reach the starting point, decrease the distance from the starting point
	if (v->ds > 0)
	{
		v->ds--;
	}
	// If it reached the starting point, decrease the distance from the destination point
	else if (v->ds == 0 && v->ride.dd > 0)
	{
		if (v->r != v->ride.a)
		{
			v->r = v->ride.a;
		}
		if (v->c != v->ride.b)
		{
			v->c = v->ride.b;
		}

		v->ride.dd--;
	}
	// If it arrived at the destination point, set it as available for another ride
	else if (v->ds == 0 && v->ride.dd == 0)
	{
		if (v->av == 1)
		{
			v->r = v->ride.x;
			v->c = v->ride.y;
			v->av = 0;

			v->nrd++;

			v->idrd[v->ride.id] = 1;

			//printf("VEHICLE %d ARRIVED AT DESTINATION. \n", v->id);

			assign_single_ride_best_or_random(SMP, DIST_S, v, REW, BEST, NB, t, slots);
		}
	}
}



// Moves each vehicle to the next step
void move_vehicles(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, int NB, int t, int *slots)
{
	for (int i = 0; i < SMP->F; i++)
	{
		move_single_vehicle(SMP, DIST_S, &(SMP->vehicles[i]), REW, BEST, NB, t, slots);
	}
}


// Initializes the distances from the vehicles to the starting points
void initialize_best(mixcouple **BEST, int F, int NB)
{
	printf("INITIALIZING BEST\n");
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < NB; j++)
		{
			mixcouple c = { .val = -1,.idr = -1 };
			BEST[i][j] = c;
		}
	}
}



// Initializes the distances from the vehicles to the starting points
void initialize_best_v2(mixtriple **BEST, int F, int NB)
{
	printf("INITIALIZING BEST\n");
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < NB; j++)
		{
			mixtriple c = { .val = INT_MAX, .idr = -1, .t = INT_MAX };
			BEST[i][j] = c;
		}
	}
}


// Initializes the distances from the vehicles to the starting points
void initialize_distances_s(int **DIST_S, sample *SMP, mixcouple **BEST, couple **REW, int t, int NB, int *slots)
{
	printf("INITIALIZING DISTANCES, REWARDS AND SELECTING THE BEST ONES\n");
	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < SMP->N; j++)
		{
			DIST_S[i][j] = distance(SMP->vehicles[i].r, SMP->vehicles[i].c, SMP->rides[j].a, SMP->rides[j].b);

			int rwrd = reward(SMP->vehicles[i], SMP->rides[j], DIST_S[i][j], t, SMP->T, SMP->B);

			couple c = { .val = rwrd, .idr = SMP->rides[j].id };

			REW[i][j] = c;

			store_best_rewards(SMP, DIST_S, BEST, REW, SMP->vehicles[i].id, SMP->rides[j].id, NB, slots, t);
		}
	}
}



// Allocates the required structures (DIST_S, REW, BEST) 
void allocate_structures(int **DIST_S, couple **REW, mixcouple **BEST, sample *SMP, int NB)
{
	printf("ALLOCATING STRUCTURES\n");

	DIST_S = calloc(SMP->F, sizeof(int *));		// Distances from the vehicles to the starting points

	for (int i = 0; i < SMP->F; i++)
	{
		DIST_S[i] = calloc(SMP->N, sizeof(int));
	}

	REW = calloc(SMP->F, sizeof(couple *));		// Best rides for each vehicleehic

	for (int i = 0; i < SMP->F; i++)
	{
		REW[i] = calloc(SMP->N, sizeof(couple));
	}

	BEST = calloc(SMP->F, sizeof(mixcouple *));		// Best rides for each vehicle

	for (int i = 0; i < SMP->F; i++)
	{
		BEST[i] = calloc(NB, sizeof(mixcouple));
	}
}



// Stores the best rides according to the rewards
void store_best_rewards(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{
	// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
	if (REW[idv][idr].val > BEST[idv][0].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP ->T)
	{
		//couple ctemp = BEST[idv][i];
		mixcouple mc = { .val = REW[idv][idr].val, .idr = REW[idv][idr].idr };

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

		mixcouple c = { .val = DIST_S[idv][idr], .idr = idr };

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



// Stores the best rides according to the magic
void store_best_magic(sample *SMP, int **DIST_S, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t)
{
	for (int i = 0; i < NB; i++)
	{
		int wait = max(0, SMP->rides[idr].s - (t + DIST_S[idv][idr]));

		int bon = 0;

		if (t + DIST_S[idv][idr] <= SMP->rides[idr].s)
			bon = SMP->B;

		fcouple med = compute_average_dist(SMP->rides, SMP->N);

		int dd_med = distance(SMP->rides[idr].x, SMP->rides[idr].y, med.x, med.y);

		int magic_value = SMP->rides[idr].dd - DIST_S[idv][idr] - wait + bon - dd_med;

		couple c = { .val = magic_value,.idr = idr };

		// If the reward of assigning ride r to vehicle v is greater than the one in the current slot, update it with the greater one
		if (c.val > BEST[idv][i].val && t + DIST_S[idv][idr] + SMP->rides[idr].dd < SMP->T)
		{
			//couple ctemp = BEST[idv][i];

			BEST[idv][i] = c;

			//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
		}
	}
}



// Updates the distances in DIST_S, rewards in REW, best rides in BEST
void update_structures(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, vehicle *v, int t, int NB, int *slots)
{
	for (int i = 0; i < SMP->N; i++)
	{
		if (SMP->rides[i].done == 1)
			continue;

		DIST_S[v->id][i] = distance(v->r, v->c, SMP->rides[i].a, SMP->rides[i].b);

		int rwrd = reward(*v, SMP->rides[i], DIST_S[v->id][i], t, SMP->T, SMP->B);

		couple c = { .val = rwrd,.idr = SMP->rides[i].id };

		REW[v->id][i] = c;

		mixcouple mc = { .val = -1,.idr = -1 };

		BEST[v->id][0] = mc;

		store_best_rewards(SMP, DIST_S, BEST, REW, v->id, i, NB, slots, t);
	}
}


fcouple compute_average_dist(ride *rides, int n)
{
	float x = 0;
	float y = 0;

	for (int i = 0; i < n; i++)
	{
		x += rides[i].a;
		y += rides[i].b;
	}
	
	fcouple c = { .x = x/n, .y = y/n };

	return c;
}