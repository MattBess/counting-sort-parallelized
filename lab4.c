#include<mpi.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

int findMaxElement(int A[], int);
void countingSort(int A[], int sortedA[], int, int);

int main(int argc, char * argv[]){
	
	MPI_Init(&argc, &argv);
	
	int id, rank, i, root = 0;
	int size = 10000;
	int max = 300;
	int A[size], sortedArray[size], globalArray[size];
	
	//The id of the processor
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	//Total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &rank);
	
	//Initialize array
	if(id == 0){
		for(i = 0; i < size; i++){
			A[i] = rand() % 300;
		}
	}
	
	//The amount of elements to send to each processor
	int sendCount = size / rank;
	int recvCount = sendCount;
	int localA[sendCount], recvSortedArrays[rank];
	
	//Evenly scattering the elements across all processors
	MPI_Scatter(A, sendCount, MPI_INT, localA, recvCount, MPI_INT, root, MPI_COMM_WORLD);
	
	//Sort the arrays
	int localSortedArray[recvCount];
	countingSort(localA, localSortedArray, max, recvCount);
	
	if(rank == 0){
		
	}
	
	//Gather all sorted arrays
	MPI_Gather(localSortedArray, recvCount, MPI_INT, recvSortedArrays[rank], recvCount, MPI_INT, root, MPI_COMM_WORLD);
	
	if(id == 0){
		for(i = 0; i 
	}
	
	MPI_Finalize();
}

int findMaxElement(int A[], int size){
	int max = A[0];
	int i = 0;
	for(i = 1; i < size; i++){
		if(A[i] > max) max = A[i];
	}
	
	return max;
}

void countingSort(int A[], int sortedA[], int max, int size){
	//This array provides temporary working storage
	int C[max];
	int i = 0;
	for (i = 0; i < k; i++) {
		C[i] = 0;
	}

	//{95, 1, 37, 27, 27}
	//Counting the # of elements equal to each other in A
	for (i = 0; i < size; i++) {
		C[A[i]] = C[A[i]] + 1;
	}
	//C[i] now contains the number of elements equal to i

	//Count the # of elements less than or equal to i
	for (i = 1; i < k; i++) {
		C[i] = C[i] + C[i - 1];
	}
	//C[i] now contains the # of elements <= to i

	//This is where the sorting happens
	for (i = size-1; i >= 0; i--) {
		//Find the # of elements <= to value at index A[i]
		sortedArray[C[A[i]]] = A[i];
		//Decrement the # of elements <= to value at index A[i} because it's been added to sorted array
		C[A[i]] = C[A[i]] - 1;
	}
}