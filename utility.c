#include "utility.h"
#include "heuristics.h"

void load_data(char *data_file, sample *SMP)
{
	printf("LOADING DATA \n");

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	char delim[] = " ";

	fp = fopen(data_file, "r");

	if (fp == NULL)
	{
		fprintf(stderr, "File %s could not be opened!\n", data_file);
		exit(EXIT_FAILURE);
	}

	int i = 0;
	int split_line[6];

	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Retrieved line of length %zu:\n", read);
		//printf("%s", line);

		split_str(line, delim, split_line);

		if (i == 0)
		{
			SMP->R = split_line[0];
			SMP->C = split_line[1];
			SMP->F = split_line[2];
			SMP->N = split_line[3];
			SMP->B = split_line[4];
			SMP->T = split_line[5];
			SMP->rides = (ride *)calloc(SMP->N, sizeof(ride));
			SMP->vehicles = (vehicle *)calloc(SMP->N, sizeof(vehicle));
		}
		else
		{
			SMP->rides[i-1].a = split_line[0];
			SMP->rides[i-1].b = split_line[1];
			SMP->rides[i-1].x = split_line[2];
			SMP->rides[i-1].y = split_line[3];
			SMP->rides[i-1].s = split_line[4];
			SMP->rides[i-1].f = split_line[5];
			SMP->rides[i-1].dd = distance(SMP->rides[i - 1].a, SMP->rides[i - 1].b, SMP->rides[i - 1].x, SMP->rides[i - 1].y);
			SMP->rides[i-1].dt = -1;
			SMP->rides[i-1].done = 0;
			SMP->rides[i-1].id = i-1;

			SMP->vehicles[i-1].r = 0;
			SMP->vehicles[i-1].c = 0;
			//SMP->vehicles[i-1].ride = NULL;
			//SMP->vehicles[i-1].ds = 0;
			SMP->vehicles[i-1].av = 0;
			SMP->vehicles[i-1].id = i-1;
			SMP->vehicles[i-1].nrd = 0;
			SMP->vehicles[i-1].idrd = calloc(SMP->N, sizeof(int));

			for (int j = 0; j < SMP->N; j++)
			{
				SMP->vehicles[i - 1].idrd[j] = -1;
			}
		}

		i++;
	}

	fclose(fp);
	if (line)
		free(line);
}




void split_str(char *string, char *delim, int *split_string)
{
	char *ptr = strtok(string, delim);

	int i = 0;

	while (ptr != NULL)
	{
		if (i > 5)
			break;

		//printf("%s\n", ptr);

		split_string[i] = (int)strtol(ptr, (char **)NULL, 10);

		ptr = strtok(NULL, delim);

		i++;
	}
}


// Stores the output in the file data_file
void store_output(char *data_file, sample *SMP)
{
	printf("STORING DATA \n");

	FILE * fp;

	fp = fopen(data_file, "w");

	setbuf(fp, NULL);

	if (fp == NULL) return;

	for (int i = 0; i < SMP->F; i++)
	{
		//printf("%d ", SMP->vehicles[i].nrd);
		
		fprintf(fp, "%d ", SMP->vehicles[i].nrd);

		for (int j = 0; j < SMP->N; j++)
		{
			if (SMP->vehicles[i].idrd[j] == 1)
			{
				//printf("%d ", j);
				fprintf(fp, "%d ", j);
			}
		}

		//printf("\n");
		fprintf(fp, "\n");
	}

	fclose(fp);
}


// Stores the output in the file data_file
void store_output_v3(char *data_file, sample *SMP)
{
	printf("STORING DATA \n");

	FILE * fp;

	fp = fopen(data_file, "w");

	setbuf(fp, NULL);

	if (fp == NULL) return;

	for (int i = 0; i < SMP->F; i++)
	{
		//printf("%d ", SMP->vehicles[i].nrd);

		fprintf(fp, "%d ", SMP->vehicles[i].nrd);

		for (int j = 0; j < SMP->N; j++)
		{
			if (SMP->vehicles[i].idrd[j] == -1)
				break;

			fprintf(fp, "%d ", SMP->vehicles[i].idrd[j]);
		}

		//printf("\n");
		fprintf(fp, "\n");
	}

	fclose(fp);
}

// Computes the score, given an output file with the required format
void compute_score(char *output_file, sample *SMP)
{
	printf("COMPUTING SCORE: ");

	int total_score = 0;

	for (int i = 0; i < SMP->F; i++)
	{
		int v_x = 0;
		int v_y = 0;
		int v_score = 0;

		//printf("VEHICLE %d:\n", i);

		for (int j = 0; j < SMP->N; j++)
		{
			if (SMP->vehicles[i].idrd[j] == 0)
				continue;
			//printf("RIDE: %d\n", SMP->rides[j].id);
			int r_score = 0;
			int ds = distance(v_x, v_y, SMP->rides[j].a, SMP->rides[j].b);

			// BONUS POINTS FOR ARRIVING IN TIME
			if (ds <= SMP->rides[j].s)
				r_score += SMP->B;

			//printf("VEHICLE: %d\nBONUS: %d ", i, r_score);

			// POINTS FOR THE RIDE IN TIME
			if (ds + SMP->rides[j].dd < SMP->rides[j].f)
				r_score += SMP->rides[j].dd;

			//printf("DISTANCE: %d\n", SMP->rides[j].dd);

			//printf("POINTS: %d\n", r_score);

			v_x = SMP->rides[j].x;
			v_y = SMP->rides[j].y;

			v_score += r_score;

			//printf("RIDE %d, SCORE %d, DS %d, S %d, WT %d, DT %d, F %d, DD %d, BONUS %d\n", j, r_score, ds, SMP->rides[j].s, wt, t, SMP->rides[j].f, SMP->rides[j].dd, SMP->B);
		}

		//printf("VEHICLE SCORE: %d\n", v_score);

		total_score += v_score;
		//printf("VEHICLE %d, SCORE %d\n", i, v_score);
	}
	printf("%d", total_score);

	printf(" EXPERIMENTAL");
}



// Computes the score, given an output file with the required format
void compute_score_v2(char *output_file, sample *SMP)
{
	printf("COMPUTING SCORE: ");

	int total_score = 0;

	for (int i = 0; i < SMP->F; i++)
	{
		int v_x = 0;
		int v_y = 0;
		int v_score = 0;

		int t = 0;

		for (int j = 0; j < SMP->N; j++)
		{
			if (SMP->vehicles[i].idrd[j] == 0)
				continue;

			ride ride = SMP->rides[j];
			int r_score = 0;
			int ds = distance(v_x, v_y, ride.a, ride.b);

			int wt = 0;

			// BONUS POINTS FOR ARRIVING IN TIME
			if (t + ds <= ride.s)
			{
				r_score += SMP->B;
				wt = max(0, ride.s - (t + ds));
			}

			//printf("VEHICLE: %d\nBONUS: %d ", i, r_score);

			int required_time = t + ds + wt + ride.dd;

			// POINTS FOR THE RIDE IN TIME
			if (required_time <= ride.f && required_time <= SMP->T)
				r_score += ride.dd;

			//printf("DISTANCE: %d\n", SMP->rides[j].dd);

			//printf("POINTS: %d\n", r_score);

			v_x = SMP->rides[j].x;
			v_y = SMP->rides[j].y;

			v_score += r_score;

			t = required_time;
		}

		//printf("VEHICLE SCORE: %d\n", v_score);

		total_score += v_score;
	}
	printf("%d", total_score);

	printf(" EXPERIMENTAL");
}


// Computes the score, given an output file with the required format
int compute_score_v3(char *output_file, sample *SMP)
{
	printf("COMPUTING SCORE: ");

	int total_score = 0;

	for (int i = 0; i < SMP->F; i++)
	{
		int v_x = 0;
		int v_y = 0;
		int v_score = 0;

		int t = 0;

		//printf("VEHICLE %d:\n", i);

		for (int j = 0; j < SMP->N; j++)
		{
			int idr = SMP->vehicles[i].idrd[j];
			if (idr == -1)
				break;

			//printf("RIDE: %d\n", SMP->rides[idr].id);
			int r_score = 0;
			int ds = distance(v_x, v_y, SMP->rides[idr].a, SMP->rides[idr].b);
			int wt = max(0, SMP->rides[idr].s - (t + ds));

			// BONUS POINTS FOR ARRIVING IN TIME
			if (t + ds <= SMP->rides[idr].s)
				r_score += SMP->B;

			int total_time = t + ds + wt + SMP->rides[idr].dd;

			//printf("VEHICLE: %d\nBONUS: %d ", i, r_score);

			// POINTS FOR THE RIDE IN TIME
			if (total_time <= SMP->rides[idr].f)
				r_score += SMP->rides[idr].dd;

			//printf("DISTANCE: %d\n", SMP->rides[j].dd);

			//printf("POINTS: %d, T: %d, DS: %d, TDS: %d, S: %d, DD: %d, TT: %d, F: %d\n", r_score, t, ds,  t + ds, SMP->rides[idr].s, SMP->rides[idr].dd, total_time, SMP->rides[idr].f);

			v_x = SMP->rides[idr].x;
			v_y = SMP->rides[idr].y;

			v_score += r_score;

			t = total_time;

			//printf("RIDE %d, SCORE %d, DS %d, S %d, WT %d, DT %d, F %d, DD %d, BONUS %d\n", j, r_score, ds, SMP->rides[j].s, wt, t, SMP->rides[j].f, SMP->rides[j].dd, SMP->B);
		}

		//printf("VEHICLE SCORE: %d\n", v_score);

		total_score += v_score;
		//printf("VEHICLE %d, SCORE %d\n", i, v_score);
	}
	printf("%d", total_score);

	return total_score;
}


void freeMemory(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int NB)
{
	for (int i = 0; i < SMP->F; i++)
	{
		free(DIST_S[i]);
		//free(WAIT[i]);

		//free(&SMP->vehicles[i].idrd);
		//free(&SMP->vehicles[i].ride);

		free(REW[i]);
		free(BEST[i]);
	}
	free(DIST_S);
	//free(WAIT);
	free(REW);
	free(BEST);

	//free(&SMP->B);
	//free(&SMP->C);
	//free(&SMP->F);
	//free(&SMP->N);
	//free(&SMP->R);
	//free(&SMP->T);
	//free(SMP->rides);
	//free(SMP->vehicles);
	//free(SMP);
}