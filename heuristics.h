#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include "utility.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// Computes and returns the distance between beginning intersection [a,b] and destination intersection [x,y]
int distance(int a, int b, int x, int y);

float avg_distance(sample *SMP, int xs, int ys);

// Returns the reward given by assigning the ride r to the vehicle v at time t
int reward(vehicle v, ride r, int dist, int t, int T, int B);

// Returns the reward given by assigning the ride r to the vehicle v at time t
int reward_v3(sample *SMP, int **DIST_S, vehicle v, ride r, int t);

// Returns the wait_time for the couple (vehicle, ride)
int wait_time(sample *SMP, int **DIST_S, int idv, int idr, int t);

// Returns the late_time for the couple (vehicle, ride)
int late_time(sample *SMP, int **DIST_S, int idv, int idr, int t);

// Returns the variance of an element
float variance_required_time(sample *SMP, int **REW, int **DIST_S, int **WAIT, int idv, int t, int required_time);

// Returns the variance of an element
float variance_reward(sample *SMP, int **REW, int idv, int idr);



/////////////////////////////////////////////////// VERSION 2 ///////////////////////////////////////////////////

// Initializes the distances from the vehicles to the starting points
void initialize_best_v2(sample *SMP, mixtriple **BEST, int F, int NB, int crit);

// Initializes the values in BEST for each ride of a given vehicle v
void initialize_best_v2v_min_max(mixtriple **BEST, int F, int NB, int idv);

// Initializes the values in BEST for each ride of a given vehicle v
void initialize_best_v2v_max_max(mixtriple **BEST, int F, int NB, int idv);

// Initializes the required structures for v2
void initialize_structures(sample *SMP, int **DIST_S, mixtriple **BEST, int **REW, int t, int crit, int K);

// Starts a simulation with wait times
void start_simulation_v2(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int NB, int *slots, int iter, int crit, int q);

// Assigns a ride according to a choice_criterium and returns the time required to complete the ride
int assign_ride(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int t, int iter, int q, int K);

// Makes an entire ride
void move(sample *SMP, int idv, int t);

// Updates the necessary structures
void update_rides(sample *SMP, int **DIST_S, int **REW, mixtriple **BEST, int idv, int t, int crit, int K);



/////////////////////////////////////////////////// VERSION 1 ///////////////////////////////////////////////////

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

// Updates the values in the distance matrix DIST, the rewards in the matrix REW and the best rides for the vehicles in the matrix BEST
void update_structures(sample *SMP, int **DIST_S, couple **REW, mixcouple **BEST, vehicle *v, int t, int NB, int *slots);


#endif   /* __HEURISTICS_H */