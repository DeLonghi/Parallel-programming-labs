#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char *argv[])

{
    MPI_Init(&argc, &argv);
    int N = atoi(argv[1]);
    double time_start, time_finish;
    time_start = MPI_Wtime();
    char *name = new char;
    int len;
    int rank, n, i;
    int *vec1;
    int *vec2;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    int n_bins = N / n;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
    if (rank == 0)
    {
        vec1 = new int[N];
        vec2 = new int[N];

        for (i = 0; i < N; i++)
        {
            vec1[i] = 1;
            vec2[i] = 1;
        }
        for (i = 1; i < n; ++i)
        {
            MPI_Send(&vec1[n_bins * i], n_bins, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&vec2[n_bins * i], n_bins, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        vec1 = new int[n_bins]{0};
        vec2 = new int[n_bins]{0};
        MPI_Recv(&(vec1[0]), n_bins, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&(vec2[0]), n_bins, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    int sum = 0;
    for (i = 0; i < n_bins; i++)
        sum += vec1[i] * vec2[i];
    cout << "process: " << rank
         << "   FROM: " << n_bins * rank
         << "   TO: " << n_bins * (rank + 1)
         << "   SUM: " << sum << endl;
    time_finish = MPI_Wtime();
    int total = 0;
    MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
        cout << "TOTAL SUM: " << total;
    MPI_Finalize();
    return 0;
}