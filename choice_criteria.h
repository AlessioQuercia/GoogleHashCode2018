#ifndef __CHOICE_CRITERIA_H
#define __CHOICE_CRITERIA_H

#include "utility.h"
#include "heuristics.h"

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


#endif