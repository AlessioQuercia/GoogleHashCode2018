# Google Hash Code 2018
## Overview

The repository is intended to keep track of files needed to solve Google HashCode 2018 with a heuristic approach for the project related to the Heuristic Algorithms course at Università degli Studi di Milano. 

## Info
The project shows a possible approach to the Google Hash Code 2018 Online Qualification round problem, that is an optimization problem to be solved within 4 hours. This constraint has been ignored, given that this approach has been developed after the competition for academic purposes. The main goal was to solve the problem in an efficient way, using an heuristic (or metaheuristic) approach. To achieve this goal, different greedy approaches have been developed and compared by looking at their results. Then, the best performing one has been used as a base to build a GRASP (Greedy Randomized Adaptive Search Procedure) heuristic approach. Indeed, randomness has been added to the greedy in the choice criterion, allowing the algorithm to make choices that are not always the best ones.

For more info, read the report in the Links section.

## Score

### Greedy
| Sample           | Result   | Time (s) |
| ---------------- | -------- | -------- |
| a_example        | 10       | 0.004    |
| b_should_be_easy | 173977   | 0.018    |
| c_no_hurry       | 15809027 | 3.713    |
| d_metropolis     | 11209219 | 3.941    |
| e_high_bonus     | 21068945 | 3.989    |

**Total score**: 48261178.

**Total execution time**: 12 seconds.

### GRASP
| Sample           | Result   | Time (s) |
| ---------------- | -------- | -------- |
| a_example        | 10       | 0.003    |
| b_should_be_easy | 173977   | 0.015    |
| c_no_hurry       | 15809650 | 4.057    |
| d_metropolis     | 11254423 | 4.549    |
| e_high_bonus     | 21099945 | 4.237    |

**Total score**: 48338005.

**Total execution time**: 20 minutes.

## Links

**Report**: https://github.com/AlessioQuercia/GoogleHashCode2018/blob/master/GoogleHashCode2018_AnApproach.pdf

**Google Hash Code 2018**: https://github.com/AlessioQuercia/GoogleHashCode2018/blob/master/hashcode2018_qualification_task.pdf