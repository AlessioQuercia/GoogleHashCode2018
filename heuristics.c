#include "heuristics.h"

int distance(int a, int b, int x, int y)
{
	return abs(a - x) + abs(b - y);
}



void start_simulation(sample *SMP, int **DIST_S, int **BEST_D, int **BEST_IDR, int B)
{
	int t = 0;

	while (t < SMP->T)
	{
		printf("Time: %d\n", t);

		t++;
	}

	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < SMP->N; j++)
		{
			printf("%d ", DIST_S[i][j]);
		}
		printf("\n");
	}

	for (int i = 0; i < SMP->N; i++)
	{
		printf("%d\n", SMP->rides[i].dd);
	}

	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < B; j++)
		{
			printf("%d ", BEST_IDR[i][j]);
		}
		printf("\n");
	}
}



void assign_single_ride(vehicle *v, ride r, int **DIST_S, int **BEST_D, int **BEST_IDR)
{
	v->ride = r;
	v->ds = DIST_S[v->id][BEST_IDR[v->id][r.id]];
}



void assign_rides(int **DIST_S, int **BEST_D, int **BEST_IDR)
{

}



void update_distances(int **DIST_S, int **BEST_D, int **BEST_IDR)
{

}



// Moves the vehicle v to the next step
void move_single_vehicle(vehicle *v)
{
	// If it still has to reach the starting point, decrease the distance from the starting point
	if (v->ds > 0)
	{
		v->ds--;
	}
	// If it reached the starting point, decrease the distance from the destination point
	else if (v->ds = 0 && v->ride.dd > 0)
	{
		v->r = v->ride.a;
		v->c = v->ride.b;
		v->ride.dd--;
	}
	// If it arrived at the destination point, set it as available for another ride
	else
	{
		v->r = v->ride.x;
		v->c = v->ride.y;
		v->av = 0;
	}
}



// Moves each vehicle to the next step
void move_vehicles(sample *SMP)
{
	for (int i = 0; i < SMP->N; i++)
	{
		move_single_vehicle(&(SMP->vehicles[i]));
	}
}



// Initializes the distances from the vehicles to the starting points
void initialize_best(int **BEST_D, int **BEST_IDR, int F, int B)
{
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < B; j++)
		{
			BEST_D[i][j] = INT_MAX;

			BEST_IDR[i][j] = -1;
		}
	}
}



// Initializes the distances from the vehicles to the starting points
void initialize_distances_s(int **DIST_S, sample *SMP, int **BEST_D, int **BEST_IDR, int B)
{
	for (int i = 0; i < SMP->F; i++)
	{
		for (int j = 0; j < SMP->N; j++)
		{
			DIST_S[i][j] = distance(SMP->vehicles[i].r, SMP->vehicles[i].c, SMP->rides[j].a, SMP->rides[j].b);

			assign_if_best(BEST_D, BEST_IDR, SMP->rides[j], DIST_S[i][j], SMP->vehicles[i].id, SMP->rides[j].id, B);
		}
	}
}



// Assigns the id of the ride to the BEST ones, if it is considered one of the best B
void assign_if_best(int **BEST_D, int **BEST_IDR, ride r, int dist, int idv, int idr, int B)
{
	for (int i = 0; i < B; i++)
	{
		// If there is an empty slot, assign the ride to the best
		if (BEST_IDR[idv][i] == -1)
		{
			BEST_IDR[idv][i] = idr;
			BEST_D[idv][i] = dist;
			break;
		}
		else
		{
			// if the distance is smaller than the earliest start, assign the ride to the best
			if (dist <= r.s)
			{

			}
		}
	}
}
