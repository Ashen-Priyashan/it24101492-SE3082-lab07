Exercise 8: the root Makefile builds all six MPI programs and `make run` executes each with four processes by default. Use `make PROCESSES=8 run` to select another valid process count. `make clean` removes the built executables.

Windows setup instructions for MSYS2 and MS-MPI are in SETUP.md. On Linux, install an MPI implementation and use `make` followed by `make run`.
