#include "utility.h"

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
		}

		//printf("VEHICLE SCORE: %d\n", v_score);

		total_score += v_score;
	}
	printf("%d", total_score);

	printf(" EXPERIMENTAL");
}