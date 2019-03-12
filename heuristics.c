#include "heuristics.h"


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



void start_simulation(sample *SMP, int **DIST_S, couple **REW, couple **BEST, int NB)
{
	int t = 0;

	while (t < SMP->T)
	{
		//printf("Time: %d\n", t);

		// Move each vehicle of 1 step
		move_vehicles(SMP, DIST_S, BEST, NB);

		t++;
	}

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



void assign_single_ride(sample *SMP, int **DIST_S, vehicle *v, couple **BEST, int NB)
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

			// Updates the distances in DIST_S and the rewards in REW to be assigned when the vehicle reaches its destination
			//update_distances();

			// Updates the best rides to be assigned when the vehicle reaches its destination
			//update_best();

			//printf("RIDE %d ASSIGNED AT VEHICLE %d.\n", BEST[v->id][i].idr, v->id);

			break;
		}
	}
}



void assign_rides(sample *SMP, int **DIST_S, couple **BEST, int NB)
{
	printf("ASSIGNING RIDES\n");
	for (int i = 0; i < SMP->F; i++)
	{
		//printf("VEHICLE %d: ", i);

		assign_single_ride(SMP, DIST_S, &(SMP->vehicles[i]), BEST, NB);
	}
}



void update_distances(int **DIST_S, couple **BEST)
{

}



// Moves the vehicle v to the next step
void move_single_vehicle(sample *SMP, int **DIST_S, vehicle *v, couple **BEST, int NB)
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

			assign_single_ride(SMP, DIST_S, v, BEST, NB);
		}
	}
}



// Moves each vehicle to the next step
void move_vehicles(sample *SMP, int **DIST_S, couple **BEST, int NB)
{
	for (int i = 0; i < SMP->F; i++)
	{
		move_single_vehicle(SMP, DIST_S, &(SMP->vehicles[i]), BEST, NB);
	}
}



// Initializes the distances from the vehicles to the starting points
void initialize_best(couple **BEST, int F, int NB)
{
	printf("INITIALIZING BEST\n");
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < NB; j++)
		{
			couple c = { .rew = -1, .idr = -1 };
			BEST[i][j] = c;
		}
	}
}


// Initializes the distances from the vehicles to the starting points
void initialize_distances_s(int **DIST_S, sample *SMP, couple **BEST, couple **REW, int t, int NB, int *slots)
{
	printf("INITIALIZING DISTANCES\n");
	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < SMP->N; j++)
		{
			DIST_S[i][j] = distance(SMP->vehicles[i].r, SMP->vehicles[i].c, SMP->rides[j].a, SMP->rides[j].b);

			int rwrd = reward(SMP->vehicles[i], SMP->rides[j], DIST_S[i][j], t, SMP->T, SMP->B);

			couple c = { .rew = rwrd, .idr = SMP->rides[j].id };

			REW[i][j] = c;

			assign_if_best(SMP, BEST, REW, SMP->vehicles[i].id, SMP->rides[j].id, NB, slots);
		}
	}
}



// Allocates the required structures (DIST_S, REW, BEST) 
void allocate_structures(int **DIST_S, couple **REW, couple **BEST, sample *SMP, int NB)
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

	BEST = calloc(SMP->F, sizeof(couple *));		// Best rides for each vehicle

	for (int i = 0; i < SMP->F; i++)
	{
		BEST[i] = calloc(NB, sizeof(couple));
	}
}



// Assigns the id of the ride to the BEST ones, if it is considered one of the best B
void assign_if_best(sample *SMP, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots)
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
				if (REW[idv][idr].rew > BEST[idv][i].rew)
				{
					//couple ctemp = BEST[idv][i];

					BEST[idv][i] = REW[idv][idr];

					//assign_if_best(SMP, BEST, REW, idv, ctemp.idr, NB, slots);
				}
			}
		}
	}
}
