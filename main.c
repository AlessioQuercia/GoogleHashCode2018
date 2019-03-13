#include "heuristics.h"

void main()
{
	sample SMP;		// Sample

	//char input[] = "data/a_example.in";
	//char output[] = "data/a_example.out";

	//char input[] = "data/b_should_be_easy.in";
	//char output[] = "data/b_should_be_easy.out";

	//char input[] = "data/c_no_hurry.in";
	//char output[] = "data/c_no_hurry.out";

	//char input[] = "data/d_metropolis.in";
	//char output[] = "data/d_metropolis.out";

	char input[] = "data/e_high_bonus.in";
	char output[] = "data/e_high_bonus.out";

	load_data(input, &SMP);		// Loads the sample

	int **DIST_S = calloc(SMP.F, sizeof(int *));		// Distances from the vehicles to the starting points

	for (int i = 0; i < SMP.F; i++)
	{
		DIST_S[i] = calloc(SMP.N, sizeof(int));
	}

	int NB = SMP.N;		// Number of best rides to select for each vle (B <= F <= N)

	couple **REW = calloc(SMP.F, sizeof(couple *));		// Best rides for each vehicleehic

	for (int i = 0; i < SMP.F; i++)
	{
		REW[i] = calloc(SMP.N, sizeof(couple));
	}

	couple **BEST = calloc(SMP.F, sizeof(couple *));		// Best rides for each vehicle

	for (int i = 0; i < SMP.F; i++)
	{
		BEST[i] = calloc(NB, sizeof(couple));
	}

	int slots = NB;

	int *pslots = &slots;

	initialize_best(BEST, SMP.F, NB);		// Initializes BEST

	initialize_distances_s(DIST_S, &SMP, BEST, REW, 0, NB, pslots);		// Initializes DIST_S, 

	assign_rides(&SMP, DIST_S, BEST, NB);	// Assigns the rides

	start_simulation(&SMP, DIST_S, REW, BEST, NB);	// Start the simulation

	store_output(output, &SMP);		// Stores the output

	compute_score(output, &SMP);	// Computes the total score
}