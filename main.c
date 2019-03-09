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

	int B = 2;		// Number of best rides to select for each vehicle (B <= F <= N)

	int **BEST_D = calloc(SMP.F, sizeof(int *));;		// Best rides for each vehicle

	for (int i = 0; i < SMP.F; i++)
		BEST_D[i] = calloc(B, sizeof(int));

	int **BEST_IDR = calloc(SMP.F, sizeof(int *));;		// Best rides for each vehicle

	for (int i = 0; i < SMP.F; i++)
		BEST_IDR[i] = calloc(B, sizeof(int));

	initialize_best(BEST_D, BEST_IDR, SMP.F, B);		// Initializes BEST_D and BEST_IDR

	initialize_distances_s(DIST_S, &SMP, BEST_D, BEST_IDR, B);		// Initializes DIST_S and BEST

	start_simulation(&SMP, DIST_S, BEST_D, BEST_IDR, B);	// Start the simulation
}