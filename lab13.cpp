#include <mpi.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

void alg_matmul2D(int N, float **a, float **b, float **c)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                a[i][j] += b[i][k] * c[k][j];

    return;
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    int N = atoi(argv[1]);

    double start_time, end_time;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    float **a = new float *[N];
    float **b = new float *[N];
    float **c = new float *[N];

    for (int i = 0; i < N; i++)
    {
        a[i] = new float[N];
        b[i] = new float[N];
        c[i] = new float[N];
        for (int j = 0; j < N; j++)
        {
            a[i][j] = (float)i;
            b[i][j] = (float)i;
            c[i][j] = 0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization

    start_time = MPI_Wtime();

    alg_matmul2D(N, a, b, c);

    end_time = MPI_Wtime();

    cout << "process: " << rank;
    cout << ",\texecution time: " << (end_time - start_time) << endl;
    MPI_Finalize();
}