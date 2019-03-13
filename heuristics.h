#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include "utility.h"

// Starts a simulation
void start_simulation(sample *SMP, int **DIST_S, couple **REW, couple **BEST, int B);

// Assigns a ride r to a vehicle v
void assign_single_ride(sample *SMP, int **DIST_S, vehicle *v, couple **BEST, int NB);

// Assign to each available vehicle the best ride
void assign_rides(sample *SMP, int **DIST_S, couple **BEST, int NB);

// Updates the vehicles current positions
void move_single_vehicle(sample *SMP, int **DIST_S, vehicle *v, couple **BEST, int NB);

// Updates the vehicles current positions
void move_vehicles(sample *SMP, int **DIST_S, couple **BEST, int NB);

// Allocates the required structures (DIST_S, REW, BEST) 
void allocate_structures(int **DIST_S, couple **REW, couple **BEST, sample *SMP, int NB);

// Initializes the distances from the vehicles to the starting points
void initialize_best(couple **BEST, int F, int B);

// Initializes the distances from the vehicles to the starting points
void initialize_distances_s(int **DIST_S, sample *SMP, couple **BEST, couple **REW, int t, int B, int *slots);

// Assigns the id of the ride to the BEST ones, if it is considered one of the best B
void assign_if_best(sample *SMP, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots);

// Returns the reward given by assigning the ride r to the vehicle v at time t
int reward(vehicle v, ride r, int dist, int t, int T, int B);

// Updates the values in the distance matrix DIST and the best rides for the vehicles in the matrix BEST
void update_distances();

// Updates the rewards in REW
void update_rewards();

// Updates the best rides for each vehicle in BEST
void update_best();

#endif   /* __HEURISTICS_H */