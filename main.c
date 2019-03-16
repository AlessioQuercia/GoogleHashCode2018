#include "heuristics.h"
#include <time.h>

void main()
{
	clock_t time = clock();

	sample SMP;		// Sample

	//char input[] = "data/a_example.in";
	//char output[] = "data/a_example.out";

	//char input[] = "data/b_should_be_easy.in";
	//char output[] = "data/b_should_be_easy.out";

	char input[] = "data/c_no_hurry.in";
	char output[] = "data/c_no_hurry.out";

	//char input[] = "data/d_metropolis.in";
	//char output[] = "data/d_metropolis.out";

	//char input[] = "data/e_high_bonus.in";
	//char output[] = "data/e_high_bonus.out";

	load_data(input, &SMP);		// Loads the sample

	int **DIST_S = calloc(SMP.F, sizeof(int *));		// Distances from the vehicles to the starting points

	for (int i = 0; i < SMP.F; i++)
	{
		DIST_S[i] = calloc(SMP.N, sizeof(int));
	}

	int **WAIT = calloc(SMP.F, sizeof(int *));		// Wait time for each couple (vehicle, ride)

	for (int i = 0; i < SMP.F; i++)
	{
		WAIT[i] = calloc(SMP.N, sizeof(int));
	}

	int NB = 1;		// Number of best rides to select for each vehicle (B <= F <= N)
					// 1 for the best ride for each vehicle

	couple **REW = calloc(SMP.F, sizeof(couple *));		// Best rides for each vehicleehic

	for (int i = 0; i < SMP.F; i++)
	{
		REW[i] = calloc(SMP.N, sizeof(couple));
	}

	int slots = NB;

	int *pslots = &slots;

	

	// V1

	//mixcouple **BEST = calloc(SMP.F, sizeof(mixcouple *));		// Best rides for each vehicle

	//for (int i = 0; i < SMP.F; i++)
	//{
	//	BEST[i] = calloc(NB, sizeof(mixcouple));
	//}

	//initialize_best(BEST, SMP.F, NB);		// Initializes BEST

	//initialize_distances_s(DIST_S, &SMP, BEST, REW, 0, NB, pslots);		// Initializes DIST_S, 

	//assign_rides(&SMP, DIST_S, REW, BEST, NB, 0, pslots);	// Assigns the rides

	//start_simulation(&SMP, DIST_S, REW, BEST, NB, pslots);	// Start the simulation



	// V2

	mixquad **BEST = calloc(SMP.F, sizeof(mixquad *));		// Best rides for each vehicle

	for (int i = 0; i < SMP.F; i++)
	{
		BEST[i] = calloc(NB, sizeof(mixquad));
	}

	initialize_best_v2(BEST, SMP.F, NB);		// Initializes BEST

	initialize_structures(&SMP, DIST_S, BEST, REW, WAIT, 0); // Initialize the required structures

	start_simulation_v2(&SMP, DIST_S, REW, BEST, WAIT, NB, pslots);	// Start the simulation (with wait times)


	// STORE + SCORE

	store_output(output, &SMP);		// Stores the output

	compute_score(output, &SMP);	// Computes the total score

	time = clock() - time;

	double cpu_time_used = ((double)time) / CLOCKS_PER_SEC;

	printf("\nEXECUTION TIME: %f seconds\n", cpu_time_used);
}