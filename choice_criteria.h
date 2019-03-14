#ifndef __CHOICE_CRITERIA_H
#define __CHOICE_CRITERIA_H

#include "utility.h"

// Choice criterium to store the best ride in BEST
void min_wait_time(sample *SMP, int **DIST_S, couple **REW, mixtriple **BEST, int **WAIT, int idv, int idr, int st);


#endif