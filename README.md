# Operations on Matrices using MPI

## How to run

To run the sequential code, run the following commands on the root folder:

```
g++ sequential.cpp
./a.out
```

To run the parallel code, run the following commands on the root folder:

```
mpic++ parallel.cpp
mpirun -np 4 ./a.out
```
