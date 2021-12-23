#include <stdio.h>
#include <iostream>

#include "mpi.h"
#define n 1000
using namespace std;
void smod5(void *a, void *b, int *l, MPI_Datatype *type)
{
    int i;
    for (i = 0; i < *l; i++)
        ((int *)b)[i] = (((int *)a)[i] + ((int *)b)[i]) % 5;
}

void max(void *a, void *b, int *l, MPI_Datatype *type)
{
    if (*(int *)a > *(int *)b)
        *(int *)b = *(int *)a;
}

int main(int argc, char **argv)
{
    int rank, size, i;
    int a[n];
    int b[n];
    MPI_Init(&argc, &argv);
    MPI_Op op;
    MPI_Op MY_MAX;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (i = 0; i < n; i++)
        a[i] = i + rank + 1;
    // printf("process %d a[0] = %d\n", rank, a[0]);
    MPI_Op_create(&smod5, 1, &op);
    MPI_Reduce(a, b, n, MPI_INT, op, 0, MPI_COMM_WORLD);
    MPI_Op_free(&op);

    MPI_Reduce(a, b, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("Max value MPI_MAX = %d\n", b[0]);
    MPI_Op_create(&max, 1, &MY_MAX);
    MPI_Reduce(a, b, n, MPI_INT, MY_MAX, 0, MPI_COMM_WORLD);
    MPI_Op_free(&MY_MAX);

    if (rank == 0)
        printf("Max value MY_MAX = %d\n", b[0]);

    // if (rank == 0)
    //     printf("b[0] = %d\n", b[0]);
    MPI_Finalize();
}