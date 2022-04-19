#include <mpi.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


// size of array
#define MAX 1000000

// maximum number of threads
#define MAX_THREAD 4

// size of array
#define n 1000000

// Temporary array for slave process
int a2[n];

int a[1000000];
int sum[MAX_THREAD] = { 0 };
int part = 0;

void* sum_array(void* arg)
{

	// Each thread computes sum of 1/4th of array
	int thread_part = part++;

	for (int i = thread_part * (MAX / MAX_THREAD); i < (thread_part + 1) * (MAX / MAX_THREAD); i++)
		sum[thread_part] += a[i];
    return NULL;
}


void coopProc(int argc, char* argv[])
{

	int pid, np,
		elements_per_process,
		n_elements_recieved;


	MPI_Status status;

	MPI_Init(&argc, &argv);


	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	// master process
	if (pid == 0) {
		int index, i;
		elements_per_process = n / np;

		// check if more than 1 processes are run
		if (np > 1) {
			// distributes the portion of array
			// to child processes to calculate
			// their partial sums
			for (i = 1; i < np - 1; i++) {
				index = i * elements_per_process;

				MPI_Send(&elements_per_process,
						1, MPI_INT, i, 0,
						MPI_COMM_WORLD);
				MPI_Send(&a[index],
						elements_per_process,
						MPI_INT, i, 0,
						MPI_COMM_WORLD);
			}

			// last process adds remaining elements
			index = i * elements_per_process;
			int elements_left = n - index;

			MPI_Send(&elements_left,
					1, MPI_INT,
					i, 0,
					MPI_COMM_WORLD);
			MPI_Send(&a[index],
					elements_left,
					MPI_INT, i, 0,
					MPI_COMM_WORLD);
		}

		// master process add its own sub array
		int sum = 0;
		for (i = 0; i < elements_per_process; i++)
			sum += a[i];

		// collects partial sums from other processes
		int tmp;
		for (i = 1; i < np; i++) {
			MPI_Recv(&tmp, 1, MPI_INT,
					MPI_ANY_SOURCE, 0,
					MPI_COMM_WORLD,
					&status);
			int sender = status.MPI_SOURCE;

			sum += tmp;
		}

		// prints the final sum of array
		printf("Sum of array is (using cooperating processes) : %d\n", sum);
	}
	// slave processes
	else {
		MPI_Recv(&n_elements_recieved,
				1, MPI_INT, 0, 0,
				MPI_COMM_WORLD,
				&status);

		// stores the received array segment
		// in local array a2
		MPI_Recv(&a2, n_elements_recieved,
				MPI_INT, 0, 0,
				MPI_COMM_WORLD,
				&status);

		// calculates its partial sum
		int partial_sum = 0;
		for (int i = 0; i < n_elements_recieved; i++)
			partial_sum += a2[i];

		// sends the partial sum to the root process
		MPI_Send(&partial_sum, 1, MPI_INT,
				0, 0, MPI_COMM_WORLD);
	}

	// cleans up all MPI state before exit of process
	MPI_Finalize();

}
int usingThreads()
{
    pthread_t threads[MAX_THREAD];

	// Creating 4 threads
	for (int i = 0; i < MAX_THREAD; i++)
		pthread_create(&threads[i], NULL, sum_array, (void*)NULL);

	// joining 4 threads i.e. waiting for all 4 threads to complete
	for (int i = 0; i < MAX_THREAD; i++)
		pthread_join(threads[i], NULL);

	// adding sum of all 4 parts
	int total_sum = 0;
	for (int i = 0; i < MAX_THREAD; i++)
		total_sum += sum[i];

    return total_sum;
}
int main(int argc, char* argv[])
{
	for(int i = 0; i < n; i++)
		a[i] = 1;

    clock_t t;
    t = clock();

    coopProc(argc, argv);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("Time taken using cooperating processes on openMPI : %.6f\n", time_taken);

    printf("\n");

    for(int i = 0; i < MAX; i++)
        a[i] = 1;
    t = clock();

    int total_sum = usingThreads();

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("The sum is (using pthreads): %d\n", total_sum);

    printf("Time taken using cooperating processes on pthreads: %.6f\n", time_taken);


}