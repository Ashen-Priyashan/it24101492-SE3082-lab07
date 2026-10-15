Exercise 6 solution: sum_scan.c

MPI_Scatter distributes contiguous chunks. MPI_Scan with MPI_SUM gives each rank its inclusive prefix_sum. sum_before_me = prefix_sum - local_sum is the sum of all lower-rank chunks, and each rank verifies its prefix using K*(K+1)/2. The last rank's prefix is the global sum.

Build and run with: mpicc -O2 -o sum_scan Exercise06/sum_scan.c && mpirun -np 4 ./sum_scan
