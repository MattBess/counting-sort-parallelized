#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void count(int A[], int sortedA[], int);
const int max = 32;
const int n = 512;

int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);
	MPI_Status status;
	clock_t tStart = clock();
	int id, size, i, root = 0;
	int A[n], sortedArray[n];
	//This array provides temporary working storage
    int C[n] = { 0 };
	//The id of the processor
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	//Total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//Initialize array
	if(id == 0){
		srand (time(NULL));
		for(i = 0; i < n; i++){
			A[i] = rand() % max;
			sortedArray[i] = 0;
		}
		printf("Unsorted Array:\n");
		for(i = 0; i < n; i++){
			if(i != 0 && i % 10 == 0){
				printf("%d\n", A[i]);
			}
			else{
				printf("%d, ", A[i]);
			}
		}
		printf("\n");
	}

	//The amount of elements to send to each processor
	int sendCount = n / size;
	int recvCount = sendCount;
	int localA[sendCount], localSortedArray[sendCount];
	int localC[n] = { 0 };
	
	//Evenly scattering the elements across all processors
	MPI_Scatter(A, sendCount, MPI_INT, localA, recvCount, MPI_INT, root, MPI_COMM_WORLD);
	
	//Counts the number of occuring numbers
	count(localA, localC, recvCount);
	
	//Gathers the number of occuring numbers and sums them
	MPI_Allreduce(localC, C, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Parallel prefix sum O(log n)
	int tempC[n] = { 0 };
	tempC[0] = C[0];
	MPI_Barrier(MPI_COMM_WORLD);
    for(int j = 0; j < int(log2(n)); j++){
        if(id >= int(pow(2,j))){
            tempC[id] = C[id] + C[int(id-int(pow(2,j)))];
        }
        MPI_Allreduce(tempC, C, n, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    }
    
	MPI_Reduce(C, tempC, n, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);

	//Perform one last sort from gathered sorted-subarrays
	if(id == 0){
		//Sort the array
		for (int i = 0; i <= n; i++) {
			//Find the # of elements <= to value at index A[i]
			sortedArray[C[A[i]]-1] = A[i];
			//Decrement the # of elements <= to value at index A[i} because it's been added to sorted array
			C[A[i]]--;
		}
		printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		//Print out sorted array
		printf("\n\nSorted Array:\n");
		for(int i = 0; i < n; i++){
			if(i != 0 && i % 10 == 0){
				printf("%d\n", sortedArray[i]);
			}
			else{
				printf("%d, ", sortedArray[i]);
			}
		}
		printf("\n");
	}
	MPI_Finalize();
}

void count(int localA[], int localC[], int n){
	//{95, 1, 37, 27, 27}
	//Counting the # of elements equal to each other in A
	for (int i = 0; i < n; i++) {
		localC[localA[i]]++;
	}
}