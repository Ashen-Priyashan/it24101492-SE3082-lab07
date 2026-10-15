# MPI setup

## Windows with MSYS2 MinGW-w64

This project was built with MSYS2's MinGW-w64 GCC and Microsoft MPI.

1. Install the MSYS2 MPI development package in the MinGW64 environment:
   `pacman -S mingw-w64-x86_64-msmpi`
2. Install the **MS-MPI runtime** (`msmpisetup.exe`) from the [Microsoft Download Center](https://www.microsoft.com/en-us/download/details.aspx?id=105289). The runtime provides `mpiexec`; the MSYS2 package provides `mpicc`, headers, and link libraries.
3. In PowerShell, add both tool directories to the current session's PATH:
   `$env:Path = 'C:\msys64\mingw64\bin;C:\Program Files\Microsoft MPI\Bin;' + $env:Path`
4. From the repository directory, build and run:
   `mingw32-make all`
   `mingw32-make PROCESSES=4 run`

Use `PROCESSES=2` or `PROCESSES=8` to compare other rank counts. `N` is 1,000,000, so the process count must divide it evenly.

## Linux

Install an MPI implementation that provides `mpicc` and `mpiexec`, then run `make` and `make PROCESSES=4 run`. The Makefile uses `mpiexec -n`, supported by MS-MPI and common Linux MPI implementations.

The MSYS2 MPI package details are listed [here](https://packages.msys2.org/packages/mingw-w64-x86_64-msmpi?repo=mingw64); Microsoft documents the [MS-MPI runtime and SDK](https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi-release-notes).
