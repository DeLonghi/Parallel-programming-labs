#include <stdio.h>
#include <fstream>
#include "mpi.h"
#define BUF_SIZE 20

void createFile()
{
	std::ofstream outfile("file1.txt");
	outfile << "Look, I was gonna go easy on you Not to hurt your feelings But I'm only going to get this one chance Something's wrong, I can feel it" << std::endl;
	outfile.close();
	return;
}

int main(int argc, char **argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	char buf[BUF_SIZE];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	createFile();
	MPI_File_open(MPI_COMM_WORLD, "file1.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, rank * BUF_SIZE, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh, buf, BUF_SIZE, MPI_CHAR, MPI_STATUS_IGNORE);
	printf("process %d, buf=%s\n", rank, buf);
	MPI_File_close(&fh);
	MPI_Finalize();
}