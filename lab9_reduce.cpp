#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = atoi(argv[1]);
    int *a;
    int sub_sum = 0;
    int sum = 0;

    if (rank == 0)
    {
        a = new int[n];
        fill(a, a + n, 1);
    }

    int *sub_a = new int[n / size];
    double start_time = MPI_Wtime();
    MPI_Scatter(a, n / size, MPI_INT, sub_a, n / size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n / size; i++)
    {
        sub_sum += sub_a[i];
    }

    MPI_Reduce(&sub_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        double elapsed_time = MPI_Wtime() - start_time;
        cout << "Reduce" << endl
             << "   Sum:                " << sum << endl;
        cout << "   Execution time:     " << elapsed_time << endl;
    }

    MPI_Finalize();
    return 0;
}