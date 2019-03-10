#include "heuristics.h"

void main()
{
	sample SMP;		// Sample

	char data_file[] = "data/a_example.in";
	//char data_file[] = "data/b_should_be_easy.in";

	load_data(data_file, &SMP);		// Loads the sample

	int **DIST_S = calloc(SMP.F, sizeof(int *));		// Distances from the vehicles to the starting points

	for (int i=0; i<SMP.F; i++)
		DIST_S[i] = calloc(SMP.N, sizeof(int));

	int NB = SMP.F;		// Number of best rides to select for each vehicle (B <= F <= N)

	couple **REW = calloc(SMP.F, sizeof(int *));		// Best rides for each vehicle

	for (int i = 0; i < SMP.F; i++)
		REW[i] = calloc(SMP.N, sizeof(int));

	couple **BEST = calloc(SMP.F, sizeof(int *));		// Best rides for each vehicle

	for (int i = 0; i < SMP.F; i++)
		BEST[i] = calloc(NB, sizeof(int));

	int slots = NB;

	int *pslots = &slots;

	initialize_best(BEST, SMP.F, NB);		// Initializes BEST_D and BEST_IDR

	initialize_distances_s(DIST_S, &SMP, BEST, REW, 0, NB, pslots);		// Initializes DIST_S and BEST

	assign_rides(&SMP, DIST_S, BEST, NB);

	start_simulation(&SMP, DIST_S, REW, BEST, NB);	// Start the simulation
}