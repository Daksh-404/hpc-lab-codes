#include <stdio.h> 
#include "mpi.h"

int main(int argc, char** argv)
{
    int commsize;
    int rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	printf("Hello World from Process no. %d\n", rank);
    MPI_Finalize();

	return 0; 
}
