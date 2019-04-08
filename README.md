# Google Hash Code 2018
## Overview

The repository is intended to keep track of files needed to solve Google HashCode 2018 with a heuristic approach for the project related to the Heuristic Algorithms course at Università degli Studi di Milano. 

## Info
The project shows a possible approach to the Google Hash Code 2018 Online Qualification round problem, that is an optimization problem to be solved within 4 hours. This constraint has been ignored, given that this approach has been developed after the competition for academic purposes. The main goal was to solve the problem in an efficient way, using an heuristic (or metaheuristic) approach. To achieve this goal, different greedy approaches have been developed and compared by looking at their results. Then, the best performing one has been used as a base to build a GRASP (Greedy Randomized Adaptive Search Procedure) heuristic approach. Indeed, randomness has been added to the greedy in the choice criterion, allowing the algorithm to make choices that are not always the best ones.

For more info, read the report in the Links section.

## Score

### Greedy
| Sample           | Result   | Time (s) |
|------------------|----------|----------|
| a_example        | 10       | 0.005    |
| b_should_be_easy | 172752   | 0.016    |
| c_no_hurry       | 15809027 | 3.785    |
| d_metropolis     | 11281285 | 4.053    |
| e_high_bonus     | 20646945 | 4.053    |

**Total score**: 47910019.

**Total execution time**: 12 seconds.

### GRASP
| Sample           | Result   | Time (s) |
|------------------|----------|----------|
| a_example        | 10       | 0.002    |
| b_should_be_easy | 172827   | 0.019    |
| c_no_hurry       | 15804775 | 3.970    |
| d_metropolis     | 11339771 | 4.235    |
| e_high_bonus     | 20662945 | 4.297    |

**Total score**: 47980328.

**Total execution time**: 22 minutes.

## Links

**Report**: https://github.com/AlessioQuercia/GoogleHashCode2018/blob/master/GoogleHashCode2018_AnApproach.pdf

**Google Hash Code 2018**: https://github.com/AlessioQuercia/GoogleHashCode2018/blob/master/hashcode2018_qualification_task.pdf

