#ifndef __UTILITY_H
#define __UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Structure representing a ride
typedef struct ride_s
{
	int a;   // the row of the start intersection (0 <= a < R)
	int b;   // the column of the start intersection (0 <= b < C)
	int x;   // the row of the finish intersection (0 <= x < R)
	int y;   // the column of the finish intersection (0 <= y < C)
	int s;   // the earliest start (0 <= s < T)
	int f;   // the latest finish (0 <= f <= T), (f >= s + |x-a| + |y-b|)
	int dd;   // current distance from the current ride destination point (starting from the starting point)
	int done;   // current state of the ride (0 if to do; 1 if already done)
	int id;	 // ride id (0 <= id < N)
} ride;


// Structure representing a vehicle
typedef struct vehicle_s
{
	int r;   // current row of the vehicle
	int c;   // current column of the vehicle
	ride ride;   // current ride of the vehicle
	int ds;   // current distance from the current ride starting point
	int av;   // current availability of the vehicle (0 if available; 1 if busy)
	int id;   // vehicle id (0 <= id < F)
} vehicle;


// Structure representing a sample
typedef struct sample_s
{
	int R;   // number of rows of the grid (1 <= R <= 10000)
	int C;   // number of columns of the grid (1 <= C <= 10000)
	int F;   // number of vehicles in the fleet (1 <= F <= 1000)
	int N;   // number of rides (1 <= N <= 10000)
	int B;   // per-ride bonus for starting the ride on time (1 <= B <= 10000)
	int T;   // number of steps in the simulation (1 <= T <= 10^9)
	ride *rides; // vector of the pre-booked rides
	vehicle *vehicles; // vector of the vehicles
} sample;


// Loads the data from the file data_file into the sample SMP
void load_data(char *data_file, sample *SMP);

// Splits the string
void split_str(char *string, char *delim, int *split_string);

// Computes and returns the distance between beginning intersection [a,b] and destination intersection [x,y]
int distance(int a, int b, int x, int y);

// Prints the data inside the sample SMP
// void print_data(sample *SMP);

#endif   /* __UTILITY_H */