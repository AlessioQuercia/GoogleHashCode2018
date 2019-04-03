#include "heuristics.h"
#include <time.h>

void main()
{
	// LOADING THE SAMPLES ONCE
	int nsamples = 5;
	char *inputs[] = { "data/a_example.in", "data/b_should_be_easy.in", "data/c_no_hurry.in", "data/d_metropolis.in", "data/e_high_bonus.in" };
	char *outputs[] = { "data/a_example", "data/b_should_be_easy", "data/c_no_hurry", "data/d_metropolis", "data/e_high_bonus" };

	sample SMPs[nsamples];

	for (int z = 0; z < nsamples; z++)
	{
		char *input = inputs[z];
		load_data(input, &SMPs[z]);		// Loads the sample
		free(input);
	}

	// INITIALIZING VARIABLES
	int times = 100;
	clock_t very_total_time = clock();
	int best_total_score = 0;
	int q = 100;

	// INITIALIZING RANDOM SEED
	int seed = time(NULL);
	srand(seed);

	// REPEAT THE ALGORITHM times TIMES
	for (int h = 0; h < times; h++)
	{
		clock_t total_time = clock();
		int total_score = 0;

		//sample SMP;		// Sample
		int **DIST_S;
		int **REW;
		mixtriple **BEST;

		// FOR EACH SAMPLE
		for (int z = 0; z < nsamples; z++)
		{
			clock_t sample_time = clock();
			int sample_score = 0;

			// RE-iNITIALIZE THE SAMPLE VARIABLES
			for (int i = 0; i < SMPs[z].F; i++)
			{
				SMPs[z].vehicles[i].nrd = 0;
				SMPs[z].vehicles[i].av = 0;

				for (int j = 0; j < SMPs[z].N; j++)
				{
					SMPs[z].vehicles[i].idrd[j] = -1;
				}
			}
			for (int j = 0; j < SMPs[z].N; j++)
			{
				SMPs[z].rides[j].done = 0;
			}

			//sample SMP;		// Sample

			//char input[] = "data/a_example.in";
			//char output[] = "data/a_example.out";

			//char input[] = "data/b_should_be_easy.in";
			//char output[] = "data/b_should_be_easy.out";

			//char input[] = "data/c_no_hurry.in";
			//char output[] = "data/c_no_hurry.out";

			//char input[] = "data/d_metropolis.in";
			//char output[] = "data/d_metropolis.out";

			//char input[] = "data/e_high_bonus.in";
			//char output[] = "data/e_high_bonus.out";

			//char *input = inputs[z];
			char *str = outputs[z];

			char output[100];
			sprintf(output, "%s%d.out", str, h);

			//load_data(input, &SMP);		// Loads the sample

			// INSTANTIATE THE DISTANCES MATRIX
			DIST_S = calloc(SMPs[z].F, sizeof(int *));		// Distances from the vehicles to the starting points

			for (int i = 0; i < SMPs[z].F; i++)
			{
				DIST_S[i] = calloc(SMPs[z].N, sizeof(int));
			}

			//int **WAIT = calloc(SMP.F, sizeof(int *));		// Wait time for each couple (vehicle, ride)

			//for (int i = 0; i < SMP.F; i++)
			//{
			//	WAIT[i] = calloc(SMP.N, sizeof(int));
			//}

			// K + 1
			int NB = 11;		// Number of best rides to select for each vehicle (B <= F <= N)
							// 1 for the best ride for each vehicle

			// INSTANTIATE THE REWARDS MATRIX
			int **REW = calloc(SMPs[z].F, sizeof(int *));		// Best rides for each vehicleehic

			for (int i = 0; i < SMPs[z].F; i++)
			{
				REW[i] = calloc(SMPs[z].N, sizeof(int));
				//if (REW[i] == NULL)
				//	printf("AAAAAAAAAAAAAAAAAAAAAAAAA\n");
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

			// INSTANTIATE BESTS MATRIX
			// mixtriple
			mixtriple **BEST = calloc(SMPs[z].F, sizeof(mixtriple *));		// Best rides for each vehicle

			for (int i = 0; i < SMPs[z].F; i++)
			{
				BEST[i] = calloc(NB, sizeof(mixtriple));
			}

			// mixquad
			//mixquad **BEST = calloc(SMP.F, sizeof(mixquad *));		// Best rides for each vehicle

			//for (int i = 0; i < SMP.F; i++)
			//{
			//	BEST[i] = calloc(NB, sizeof(mixquad));
			//}

			// CHOOSE THE CRITERIUM TO USE
			int crit = 0;

			//if (h > 0)
			//{
			//	double r = (double)rand() / (double)RAND_MAX;

			//	if (r >= 0.3)
			//		crit = 0;
			//	else if (r >= 0.15 && r < 0.3)
			//		crit = 1;
			//	else if (r < 0.15)
			//		crit = 2;
			//}

			// INITIALIZE BESTS MATRIX
			initialize_best_v2(&SMPs[z], BEST, SMPs[z].F, NB, crit);		// Initializes BEST

			// INITIALIZE THE OTHER STRUCTURES
			initialize_structures(&SMPs[z], DIST_S, BEST, REW, 0, crit, NB); // Initialize the required structures

			// START THE SIMULATION
			start_simulation_v2(&SMPs[z], DIST_S, REW, BEST, NB, pslots, h, crit, q);	// Start the simulation (with wait times)


			// STORE + SCORE

			// STORE THE OUTPUT
			store_output_v3(output, &SMPs[z]);		// Stores the output

			// COMPUTE THE SCORE
			sample_score = compute_score_v3(output, &SMPs[z]);	// Computes the total score

			// COMPUTE THE SAMPLE EXECUTION TIME
			sample_time = clock() - sample_time;

			// UPDATE THE TOTAL SCORE
			total_score += sample_score;

			double cpu_sample_time_used = ((double)sample_time) / CLOCKS_PER_SEC;

			printf("\nEXECUTION TIME: %f seconds\n\n", cpu_sample_time_used);

			// FREE THE MEMORY
			freeMemory(&SMPs[z], DIST_S, REW, BEST, NB);
			/*free(input);*/
			//free(output);
		}
		
		// UPDATE THE BEST TOTAL SCORE
		if (total_score > best_total_score)
		{
			best_total_score = total_score;
		}

		total_time = clock() - total_time;

		double cpu_total_time_used = ((double)total_time) / CLOCKS_PER_SEC;

		printf("TOTAL_SCORE: %d\n", total_score);
		printf("TOTAL_TIME: %f\n\n\n", cpu_total_time_used);

	}

	// COMPUTE THE OVERALL EXECUTION TIME
	very_total_time = clock() - very_total_time;

	double cpu_very_total_time_used = ((double)very_total_time) / CLOCKS_PER_SEC;

	printf("BEST_TOTAL_SCORE: %d\n", best_total_score);
	printf("VERY_TOTAL_TIME: %f\n", cpu_very_total_time_used);

}
	