#include "utility.h"

void load_data(char *data_file, sample *SMP)
{
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
		printf("%s", line);

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
			SMP->rides[i-1].done = 0;
			SMP->rides[i-1].id = i-1;

			SMP->vehicles[i-1].r = 0;
			SMP->vehicles[i-1].c = 0;
			//SMP->vehicles[i-1].ride = NULL;
			//SMP->vehicles[i-1].ds = 0;
			SMP->vehicles[i-1].av = 0;
			SMP->vehicles[i-1].id = i-1;
		}

		i++;
	}

	printf("R: %d\n", SMP->R);
	printf("C: %d\n", SMP->C);
	printf("F: %d\n", SMP->F);
	printf("N: %d\n", SMP->N);
	printf("B: %d\n", SMP->B);
	printf("T: %d\n", SMP->T);

	for (int i = 0; i < SMP->N; i++)
	{
		printf("Ride %d:\n", i);
		printf("a: %d\n", SMP->rides[i].a);
		printf("b: %d\n", SMP->rides[i].b);
		printf("x: %d\n", SMP->rides[i].x);
		printf("y: %d\n", SMP->rides[i].y);
		printf("s: %d\n", SMP->rides[i].s);
		printf("f: %d\n", SMP->rides[i].f);
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