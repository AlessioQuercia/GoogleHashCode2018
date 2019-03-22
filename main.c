#include "heuristics.h"
#include <time.h>

void main()
{
	int nsamples = 5;
	char *inputs[] = { "data/a_example.in", "data/b_should_be_easy.in", "data/c_no_hurry.in", "data/d_metropolis.in", "data/e_high_bonus.in" };
	char *outputs[] = { "data/a_example.out", "data/b_should_be_easy.out", "data/c_no_hurry.out", "data/d_metropolis.out", "data/e_high_bonus.out" };

	sample SMPs[nsamples];

	for (int z = 0; z < nsamples; z++)
	{
		char *input = inputs[z];
		load_data(input, &SMPs[z]);		// Loads the sample
		free(input);
	}


	int times = 100;
	clock_t very_total_time = clock();
	int best_total_score = 0;
	int q = 2;

	int seed = time(NULL);
	srand(seed);

	for (int h = 0; h < times; h++)
	{
		clock_t total_time = clock();
		int total_score = 0;

		//sample SMP;		// Sample
		int **DIST_S;
		int **REW;
		mixtriple **BEST;


		for (int z = 0; z < nsamples; z++)
		{
			clock_t sample_time = clock();
			int sample_score = 0;

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
			char *output = outputs[z];

			//load_data(input, &SMP);		// Loads the sample

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

			int NB = 11;		// Number of best rides to select for each vehicle (B <= F <= N)
							// 1 for the best ride for each vehicle

			int **REW = calloc(SMPs[z].F, sizeof(int *));		// Best rides for each vehicleehic

			for (int i = 0; i < SMPs[z].F; i++)
			{
				REW[i] = calloc(SMPs[z].N, sizeof(int));
				if (REW[i] == NULL)
					printf("AAAAAAAAAAAAAAAAAAAAAAAAA\n");
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

			initialize_best_v2(BEST, SMPs[z].F, NB);		// Initializes BEST

			initialize_structures(&SMPs[z], DIST_S, BEST, REW, 0, 0, NB); // Initialize the required structures

			start_simulation_v2(&SMPs[z], DIST_S, REW, BEST, NB, pslots, h, q);	// Start the simulation (with wait times)


			// STORE + SCORE

			store_output_v3(output, &SMPs[z]);		// Stores the output

			sample_score = compute_score_v3(output, &SMPs[z]);	// Computes the total score

			sample_time = clock() - sample_time;

			total_score += sample_score;

			double cpu_sample_time_used = ((double)sample_time) / CLOCKS_PER_SEC;

			printf("\nEXECUTION TIME: %f seconds\n\n", cpu_sample_time_used);

			freeMemory(&SMPs[z], DIST_S, REW, BEST, NB);
			/*free(input);*/
			free(output);
		}

		if (total_score > best_total_score)
		{
			best_total_score = total_score;
		}

		total_time = clock() - total_time;

		double cpu_total_time_used = ((double)total_time) / CLOCKS_PER_SEC;

		printf("TOTAL_SCORE: %d\n", total_score);
		printf("TOTAL_TIME: %f\n\n\n", cpu_total_time_used);

	}

	very_total_time = clock() - very_total_time;

	double cpu_very_total_time_used = ((double)very_total_time) / CLOCKS_PER_SEC;

	printf("BEST_TOTAL_SCORE: %d\n", best_total_score);
	printf("VERY_TOTAL_TIME: %f\n", cpu_very_total_time_used);

}
	