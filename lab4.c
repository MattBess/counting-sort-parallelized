#include<mpi.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char * argv[]){
	
	MPI_Init(&argc, &argv);
	
	int id, count, i, root = 0;
	
	//The id of the processor
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	//Total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &count);
	
	
	
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

	//Counting the # of elements equal to eachother in A
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