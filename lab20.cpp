#include <stdio.h>
#include <iostream>
#include <fstream>
#include "mpi.h"
#define BUFSIZE 100

void createFile()
{
    std::ofstream outfile("file.txt");
    outfile << "Hello world!" << std::endl;
    outfile.close();
    return;
}

int main(int argc, char **argv)
{
    int bufsize, num, sum;
    MPI_Init(&argc, &argv);
    bool deleteFlag = atoi(argv[1]);
    MPI_Status status;
    MPI_File fh;
    char buf[BUFSIZE];
    createFile();
    int mode = MPI_MODE_RDONLY;
    if (deleteFlag)
        mode = mode | MPI_MODE_DELETE_ON_CLOSE;
    MPI_File_open(MPI_COMM_WORLD, "file.txt", mode, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    sum = 0;
    do
    {
        MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
        MPI_Get_count(&status, MPI_CHAR, &num);
        printf("buf=%s\n", buf);
        sum += num;
    } while (num >= BUFSIZE);
    MPI_File_close(&fh);
    //print the number of read symbols sum from the file
    printf("number of read symbols from file: %d\n", sum);
    MPI_Finalize();
}