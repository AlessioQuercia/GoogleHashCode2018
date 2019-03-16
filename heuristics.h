#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include "utility.h"

// Starts a simulation
void start_simulation(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, int NB, int *slots);

// Assigns a ride r to a vehicle v
void assign_single_ride(sample *SMP, int **DIST_S, vehicle *v, couple **REW, mixcouple **BEST, int NB, int t, int *slots);

// Assign to each available vehicle the best ride
void assign_rides(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, int NB, int t, int *slots);

// Updates the vehicles current positions
void move_single_vehicle(sample *SMP, int **DIST_S, vehicle *v, couple **REW, mixcouple **BEST, int NB, int t, int *slots);

// Updates the vehicles current positions
void move_vehicles(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, int NB, int t, int *slots);

// Allocates the required structures (DIST_S, REW, BEST) 
void allocate_structures(int **DIST_S, couple **REW, mixcouple **BEST, sample *SMP, int NB);

// Initializes the distances from the vehicles to the starting points
void initialize_best(mixcouple **BEST, int F, int NB);

// Initializes the distances from the vehicles to the starting points
void initialize_distances_s(int **DIST_S, sample *SMP, mixcouple **BEST, couple **REW, int t, int B, int *slots);

// Stores the best rides according to the rewards
void store_best_rewards(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the distances from the vehicles to the rides
void store_best_distances(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the distances from the vehicles to the rides
void store_best_total_distances(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the magic
void store_best_magic(sample *SMP, int **DIST_S, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the ratio reward/time_to_wait
void store_best_rewards_time_to_wait(sample *SMP, int **DIST_S, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the ratio reward/required_time
void store_best_rewards_required_time(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Returns the reward given by assigning the ride r to the vehicle v at time t
int reward(vehicle v, ride r, int dist, int t, int T, int B);

// Returns the wait_time for the couple (vehicle, ride)
int wait_time(sample *SMP, int **DIST_S, int idv, int idr, int t);

// Updates the values in the distance matrix DIST, the rewards in the matrix REW and the best rides for the vehicles in the matrix BEST
void update_structures(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, vehicle *v, int t, int NB, int *slots);

fcouple compute_average_dist(ride *rides, int n);


// V2
// Initializes the distances from the vehicles to the starting points
void initialize_best_v2(mixquad **BEST, int F, int NB);

// Initializes the required structures for v2
void initialize_structures(sample *SMP, int **DIST_S, mixquad **BEST, couple **REW, int **WAIT, int t);

// Starts a simulation with wait times
void start_simulation_v2(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int NB, int *slots);

// Assigns a ride according to a choice_criterium and returns the time required to complete the ride
int assign_ride(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int t);

// Makes an entire ride
void move(sample *SMP, int idv);

// Updates the necessary structures
void update_rides(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int t);

#endif   /* __HEURISTICS_H */