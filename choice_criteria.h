#ifndef __CHOICE_CRITERIA_H
#define __CHOICE_CRITERIA_H

#include "utility.h"

// Choice criterium to store the best ride in BEST
void min_wait_time(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int st);


// Choice criterium to store the best ride in BEST
void min_wait_time_ratio_reward_required_time(sample *SMP, int **DIST_S, couple **REW, mixquad **BEST, int **WAIT, int idv, int idr, int t);

#endif