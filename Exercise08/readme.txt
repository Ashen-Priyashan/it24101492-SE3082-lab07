Exercise 8: the root Makefile builds all six MPI programs and `make run` executes each with four processes by default. Use `make PROCESSES=8 run` to select another valid process count. `make clean` removes the built executables.

Build/run on a Linux host with an MPI implementation installed: `make` then `make run`.
