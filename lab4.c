#include<mpi.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

int findMaxElement(int A[], int);
void countingSort(int A[], int sortedA[], int, int);

int main(int argc, char * argv[]){
	
	MPI_Init(&argc, &argv);
	clock_t tStart = clock();
	
	int id, size, i, root = 0;
	int n = 100000;
	int max = 300;
	int A[n], sortedArray[n], globalArray[n];
	
	//The id of the processor
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	//Total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//Initialize array
	if(id == 0){
		for(i = 0; i < n; i++){
			A[i] = rand() % max;
			sortedArray[i] = 0;
			globalArray[i] = 0;
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
	}
	
	//The amount of elements to send to each processor
	int sendCount = n / size;
	int recvCount = sendCount;
	int localA[sendCount], localSortedArray[sendCount];
	
	for(i = 0; i < sendCount; i++){
		localA[i] = 0;
		localSortedArray[i] = 0;
	}
	
	//Evenly scattering the elements across all processors
	MPI_Scatter(A, sendCount, MPI_INT, localA, recvCount, MPI_INT, root, MPI_COMM_WORLD);
	
	countingSort(localA, localSortedArray, max, recvCount);
	
	//At this point the local array is sorted
	
	//Gather all sorted arrays
	MPI_Gather(localSortedArray, sendCount, MPI_INT, globalArray, recvCount, MPI_INT, root, MPI_COMM_WORLD);
	
	//Perform one last sort from gathered sorted-subarrays
	if(id == 0){
		//Last sort
		countingSort(globalArray, sortedArray, max, n);
		
		printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		
		//Print out sorted array
		printf("\n\nSorted Array:\n");
		for(i = 0; i < n; i++){
			if(i != 0 && i % 10 == 0){
				printf("%d\n", sortedArray[i]);
			}
			else{
				printf("%d, ", sortedArray[i]);
			}
		}
	}
	
	MPI_Finalize();
}

int findMaxElement(int A[], int n){
	int max = A[0];
	int i = 0;
	for(i = 1; i < n; i++){
		if(A[i] > max) max = A[i];
	}
	
	return max;
}

void countingSort(int A[], int sortedA[], int max, int n){
	//This array provides temporary working storage
	int C[max];
	int i = 0;
	for (i = 0; i < max; i++) {
		C[i] = 0;
	}

	//{95, 1, 37, 27, 27}
	//Counting the # of elements equal to each other in A
	for (i = 0; i < n; i++) {
		C[A[i]] = C[A[i]] + 1;
	}
	//C[i] now contains the number of elements equal to i

	//Count the # of elements less than or equal to i
	for (i = 1; i < max; i++) {
		if(id == 0){
			if(i == 1) printf("%d - %d\n", 0, C[0]);
			printf("%d - %d\n", i, C[i]);
		}
		C[i] = C[i] + C[i - 1];
	}
	//C[i] now contains the # of elements <= to i

	//This is where the sorting happens
	for (i = n-1; i >= 0; i--) {
		//Find the # of elements <= to value at index A[i]
		sortedA[C[A[i]]] = A[i];
		//Decrement the # of elements <= to value at index A[i} because it's been added to sorted array
		C[A[i]] = C[A[i]] - 1;
	}
}