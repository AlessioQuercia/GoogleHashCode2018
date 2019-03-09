#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include "utility.h"

// Starts a simulation
void start_simulation(sample *SMP, int **DIST_S, int **BEST_D, int **BEST_IDR, int B);

// Assigns a ride r to a vehicle v
void assign_single_ride(vehicle *v, ride r, int **DIST_S, int **BEST_D, int **BEST_IDR);

// Assign to each available vehicle the best ride
void assign_rides(int **DIST_S, int **BEST_D, int **BEST_IDR);

// Updates the values in the distance matrix DIST and the best rides for the vehicles in the matrix BEST
void update_distances(int **DIST_S, int **BEST_D, int **BEST_IDR);

// Updates the vehicles current positions
void move_single_vehicle(vehicle *v);

// Updates the vehicles current positions
void move_vehicles(sample *SMP);

// Initializes the distances from the vehicles to the starting points
void initialize_best(int **BEST_D, int **BEST_IDR, int F, int B);

// Initializes the distances from the vehicles to the starting points
void initialize_distances_s(int **DIST_S, sample *SMP, int **BEST_D, int **BEST_IDR, int B);

// Assigns the id of the ride to the BEST ones, if it is considered one of the best B
void assign_if_best(int **BEST_D, int **BEST_IDR, ride r, int dist, int idv, int idr, int B);

#endif   /* __HEURISTICS_H */