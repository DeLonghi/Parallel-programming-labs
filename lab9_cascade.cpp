#include <iostream>
#include <mpi.h>

#define LENGTH 100000
using namespace std;

void fillArray(int *array, int length)
{
    for (int i = 0; i < length; i++)
        array[i] = 1;
}

int main(int argc, char **argv)
{
    int size, rank, tempSum;
    double start, delta;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    int array[LENGTH];

    if (rank == 0)
        fillArray(array, LENGTH);

    int subLength = LENGTH / size;

    int subArray[subLength];
    if (rank == 0)
        start = MPI_Wtime();
    MPI_Scatter(&array, subLength, MPI_INT, &subArray, subLength, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < subLength; i++)
        sum += subArray[i];

    for (int order = 2; order <= size; order *= 2)
    {
        if (rank % order == 0)
        {
                MPI_Recv(&tempSum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sum += tempSum;
        }
        else
        {
            MPI_Send(&sum, 1, MPI_INT, rank - (rank % order), 0, MPI_COMM_WORLD);
            break;
        }
    }

    if (rank == 0)
    {
        delta = MPI_Wtime() - start;
        cout << "Total sum:     " << sum << endl;
        cout << "Execution time:     " << delta;
    }

    MPI_Finalize();
    return 0;
}