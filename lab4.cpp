#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    double time_start, time_finish;
    time_start = MPI_Wtime();
    char *name = new char;
    int len;
    int rank, n, i;
    float message;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
    if (rank == 5)
    {
        cout << "Reciever process: " << rank << "\n";
        for (i = 1; i < n; i++)
        {
            MPI_Recv(&message, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Hello from process %.1f\n", message);
        }
    }
    else
    {
        message = (float)rank;
        MPI_Send(&message, 1, MPI_FLOAT, 5, 0, MPI_COMM_WORLD);
    }
    time_finish = MPI_Wtime();
    cout << "processor " << name << ", process " << rank << "time = " << (time_finish - time_start) << endl;
    MPI_Finalize();
    return 0;
}