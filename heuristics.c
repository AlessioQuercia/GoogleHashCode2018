#include "heuristics.h"
#include "choice_criteria.h"

// Computes and returns the distance between beginning intersection [a,b] and destination intersection [x,y]
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


// Returns the reward given by assigning the ride r to the vehicle v at time t
int reward_v3(sample *SMP, int **DIST_S, vehicle v, ride r, int dist, int t)
{
	int rew = 0;

	int tds = t + dist;

	// If the vehicle will arrive in time (or earlier) at the starting point, starting from the time t, bonus B will be added
	if (tds <= r.s)
		rew += SMP->B;

	int wt = wait_time(SMP, DIST_S, v.id, r.id, t);

	int required_time = tds + wt + r.dd;

	// If the vehicle will arrive at destination before the end T, points proportional to the distance (r.dd) will be added
	if (required_time <= r.f && required_time <= SMP->T)
		rew += r.dd;

	return rew;
}


// Returns the wait_time for the couple (vehicle, ride)
int wait_time(sample *SMP, int **DIST_S, int idv, int idr, int t)
{
	return max(0, SMP->rides[idr].s - (t + DIST_S[idv][idr]));	// max( 0, earliest_start - (current_t + distance_vechicle_ride) )
}


// Returns the variance of an element
float variance_reward(sample *SMP, int **REW, int idv, int idr)
{
	float avg = 0;
	for (int i = 0; i < SMP->N; i++)
	{
		avg += REW[idv][i];
	}
	avg = avg / SMP->N;

	return (float)(pow(REW[idv][idr] - avg, 2));
}


// Returns the variance of an element
float variance_required_time(sample *SMP, int **REW, int **DIST_S, int **WAIT, int idv, int t, int required_time)
{
	float avg = 0;
	for (int i = 0; i < SMP->N; i++)
	{
		int wt = wait_time(SMP, DIST_S, idv, i, t);
		avg += t + DIST_S[idv][i] + wt + SMP->rides[i].dd;
	}
	avg = avg / SMP->N;

	return (float)(pow(required_time - avg, 2));
}


float avg_distance(sample *SMP, int xs, int ys)
{
	float x = 0;
	float y = 0;

	for (int j = 0; j < SMP->N; j++)
	{
		x += SMP->rides[j].a;
		y += SMP->rides[j].b;
	}

	x = x / SMP->N;
	y = y / SMP->N;

	float avg_dist = abs(xs - x) + abs(ys - y);

	return avg_dist;
}



/////////////////////////////////////////////////// VERSION 2 ///////////////////////////////////////////////////

// Starts a simulation with wait times
void start_simulation_v2(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int NB, int *slots, int iter, int q)
{
	printf("SIMULATION V2 STARTED\n");

	// For each vehicle
	for (int i = 0; i < SMP->F; i++)
	{
		int t = 0;	// Current vehicle step

		update_rides(SMP, DIST_S, REW, BEST, i, t, 0, NB);	// Update vehicle possible rides

		//printf("VEHICLE %d:\n", i);

		// While it's possible, assign a new ride and increment the current step by the required_time of the assigned ride
		while ((t += assign_ride(SMP, DIST_S, REW, BEST, i, t, iter, q, NB)) <= SMP->T)	
		{

			// Move to the assigned ride destination
			move(SMP, i, t);

			// Update vehicle possible rides
			update_rides(SMP, DIST_S, REW, BEST, i, t, 0, NB);
		}
	}

	//// For each vehicle
	//for (int i = 0; i < SMP->F; i++)
	//{
	//	int t = SMP->vehicles[i].t;	// Current vehicle step

	//	if (t > SMP->T)
	//		continue;

	//	update_rides(SMP, DIST_S, REW, BEST, WAIT, i, t, 1);	// Update vehicle possible rides

	//	//printf("VEHICLE %d:\n", i);

	//	while ((t += assign_ride(SMP, DIST_S, REW, BEST, WAIT, i, t)) <= SMP->T)	// Assign a new ride (if possible) and increment the current step by the required_time of the assigned ride
	//	{

	//		// Move to the assigned ride destination
	//		move(SMP, i, t);

	//		// Update vehicle possible rides
	//		update_rides(SMP, DIST_S, REW, BEST, WAIT, i, t, 1);
	//	}
	//}

	printf("SIMULATION V2 ENDED\n");
}

// Assigns a ride according to a choice_criterium and returns the time required to complete the ride
int assign_ride(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int t, int iter, int q, int K)
{
	int required_time = INT_MAX - t;
	int idr = BEST[idv][0].idr;

	// If it's not the first time running the algorithm, add randomness to the choice criterium
	if (iter > 0 && SMP->N > 3)
	{
		double r = (double)rand() / (double)RAND_MAX;
		double index = pow(r, q) * (K-1);
		int i = round(index);
		idr = BEST[idv][i].idr;

		//printf("%d vs %d\n", BEST[idv][i].idr, BEST[idv][0].idr);
	}

	// If the *best* (or random among the best ones) ride (according to a choice_criterium) is not done yet, assign it
	if (idr >= 0 && SMP->rides[idr].done == 0)
	{
		SMP->rides[idr].done = 1;
		SMP->vehicles[idv].ride = SMP->rides[idr];
		SMP->vehicles[idv].ds = DIST_S[idv][idr];

		//printf("%d vs %d\n", idr, BEST[idv][0].idr);

		int wait_t = wait_time(SMP, DIST_S, idv, idr, t);  //wait_time(SMP, DIST_S, idv, SMP->vehicles[idv].ride.id, t);	// max( 0, earliest_start - (current_t + distance_vechicle_ride) )

		required_time = SMP->vehicles[idv].ds + wait_t + SMP->vehicles[idv].ride.dd;	// distance_vechicle_ride + wait_time + distance_ride_destination

		//printf("VEHICLE %d, RIDE %d, T %d, DS %d, WT %d, DSWT %d, DD %d, RT %d, F %d\n\n", idv, BEST[idv][0].idr, t, SMP->vehicles[idv].ds, wait_t, SMP->vehicles[idv].ds + wait_t,
		//	SMP->vehicles[idv].ride.dd, t + required_time, SMP->vehicles[idv].ride.f);

	}

	return required_time;
}

// Makes an entire ride
void move(sample *SMP, int idv, int t)
{
	SMP->vehicles[idv].r = SMP->vehicles[idv].ride.x;
	SMP->vehicles[idv].c = SMP->vehicles[idv].ride.y;
	SMP->vehicles[idv].idrd[SMP->vehicles[idv].nrd] = SMP->vehicles[idv].ride.id;
	SMP->vehicles[idv].nrd++;
	SMP->vehicles[idv].t = t;
	//SMP->vehicles[idv].idrd[SMP->vehicles[idv].ride.id] = 1;
}



void update_rides(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int t, int crit, int K)
{
	// Re-initialize BEST
	initialize_best_v2v(BEST, SMP->F, K, idv);
	//mixtriple mc = { .val = INT_MIN,.idr = -1,.t = INT_MAX };	// mixtriple
	//mixquad mc = { .val = INT_MAX,.idr = -1, .t = INT_MAX, .rt = INT_MIN };	// mixquad

	//BEST[idv][0] = mc;

	// For each ride
	for (int i = 0; i < SMP->N; i++)
	{
		if (SMP->rides[i].done == 1)
			continue;

		// Compute the distance from the vehicle current position to the considered ride
		DIST_S[idv][i] = distance(SMP->vehicles[idv].r, SMP->vehicles[idv].c, SMP->rides[i].a, SMP->rides[i].b);

		//// Compute the wait_time for the considered ride
		//WAIT[idv][i] = wait_time(SMP, DIST_S, idv, i, t);

		// Compute the reward for the considered ride
		int rwrd = reward_v3(SMP, DIST_S, SMP->vehicles[idv], SMP->rides[i], DIST_S[idv][i], t);

		REW[idv][i] = rwrd;
		
		// Choice criterium to store the best ride in BEST
		if (crit == 0)
			max_u_min_rt_K(SMP, DIST_S, REW, BEST, idv, i, t, K-1);
		//else
		//	min_rt_max_r(SMP, DIST_S, REW, BEST, WAIT, idv, i, t);
	}

	//printf("%f, %d, %d\n", BEST[idv][0].val, BEST[idv][0].idr, BEST[idv][0].t);
}


// Initializes the required structures for v2
void initialize_structures(sample *SMP, int **DIST_S, mixtriple **BEST, int **REW, int t, int crit, int K)
{
	printf("INITIALIZING STRUCTURES\n");
	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < SMP->N; j++)
		{
			if (SMP->rides[i].done == 1)
				continue;

			if (DIST_S[i] == NULL) printf("CIAO1");
			if (REW[i] == NULL) printf("CIAO2");

			DIST_S[i][j] = distance(SMP->vehicles[i].r, SMP->vehicles[i].c, SMP->rides[j].a, SMP->rides[j].b);	// Initialze distances from vehicles to rides

			//WAIT[i][j] = wait_time(SMP, DIST_S, i, j, t);	// Initialize wait_times

			int rwrd = reward_v3(SMP, DIST_S, SMP->vehicles[i], SMP->rides[j], DIST_S[i][j], t);

			REW[i][j] = rwrd;	// Initialize rewards

			// Choice criterium to store the best ride in BEST
		// Choice criterium to store the best ride in BEST
			if (crit == 0)
				max_u_min_rt_K(SMP, DIST_S, REW, BEST, i, j, t, K-1);
			//else
			//	min_rt_max_r(SMP, DIST_S, REW, BEST, WAIT, i, j, t);
		}
	}
}


// Initializes the values in BEST for each couple of vehicle and ride
void initialize_best_v2(mixtriple **BEST, int F, int NB)
{
	//printf("INITIALIZING BEST\n");
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < NB; j++)
		{
			mixtriple c = { .val = INT_MIN,.idr = -1,.t = INT_MAX };	// mixtriple
			//mixquad c = { .val = INT_MAX,.idr = -1,.t = INT_MAX, .rt = INT_MIN };	// mixquad
			BEST[i][j] = c;
		}
	}
}

// Initializes the values in BEST for each ride of a given vehicle v
void initialize_best_v2v(mixtriple **BEST, int F, int NB, int idv)
{
	//printf("INITIALIZING BEST\n");
	for (int j = 0; j < NB; j++)
	{
		mixtriple c = { .val = INT_MIN,.idr = -1,.t = INT_MAX };	// mixtriple
		//mixquad c = { .val = INT_MAX,.idr = -1,.t = INT_MAX, .rt = INT_MIN };	// mixquad
		BEST[idv][j] = c;
	}
}


/////////////////////////////////////////////////// VERSION 1 ///////////////////////////////////////////////////

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

// Assigns the best ride if available, the next available one otherwise
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


// Assigns the best ride if available, otherwise update and find the new best one and assigns it
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


// Assigns a ride to each vehicle
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