#ifndef __CHOICE_CRITERIA_H
#define __CHOICE_CRITERIA_H

#include "utility.h"
#include "heuristics.h"

/////////////////////////////////////////////////// CHOICE CRITERIA VERSION 2 ///////////////////////////////////////////////////

// Choice criterium to store the best ride in BEST: min_wait_time --> min_required_time
void min_wt_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> max_reward --> min_required_time
void min_wt_max_r_min_rt(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time
void min_wt_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time --> min_required_time
void min_wt_max_ratio_min_rt(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> max_ratio_reward/required_time --> max_reward
void min_wt_max_ratio_max_r(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> max_reward --> max_ratio_reward/required_time == min_wait_time --> max_reward --> min_required_time
void min_wt_max_r_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> min_required_time --> max_ratio_reward/required_time == min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_ratio(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t);

// Choice criterium to store the best ride in BEST: min_wait_time --> min_required_time --> max_reward
void min_wt_min_rt_max_r(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int t);


/////////////////////////////////////////////////// CHOICE CRITERIA VERSION 1 ///////////////////////////////////////////////////

// Stores the best rides according to the rewards
void store_best_rewards(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the distances from the vehicles to the rides
void store_best_distances(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the distances from the vehicles to the rides
void store_best_total_distances(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the ratio reward/time_to_wait
void store_best_rewards_time_to_wait(sample *SMP, int **DIST_S, couple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);

// Stores the best rides according to the ratio reward/required_time
void store_best_rewards_required_time(sample *SMP, int **DIST_S, mixcouple **BEST, couple **REW, int idv, int idr, int NB, int *slots, int t);


#endif