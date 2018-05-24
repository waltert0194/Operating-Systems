/*
=====================================================================
Name        	: Austin Youngblood, Will Thompson
Class         	:CPSC 3220
Project       	: 2
Date           	: Oct. 1st, 2017
=====================================================================

This program is an implementation of merge sort that uses threads.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arrayNum[100000]; //unsorted array
int sorter[100000]; //sorted array
int run = 0;  //how many times it runs

/*
Function that reads in numbers from a file that need to be sorted. EOF, or “end of file”, is used to read in the exact number of values rather than filling the array with 0’s.
*/

void readin(char* file){
	FILE* in = fopen(file,"r");
	int i = 0;
   
   while(fscanf(in,"%d",&arrayNum[i]) != EOF){
      i++;
    	run++;
	}
   if(i <= 1)
   {
     printf("Invalid input \n");
     exit(0);
   }

	fclose(in);
}
void print(int* arrayNum){
	int i;

	for (i = 0; i < run; ++i)
    	printf("%d\n",arrayNum[i]);
}

/*
	Struct that is used in the merge sort function to split the lower/left or
higher/right parts of the array.
*/

struct index{
	int low,high;
};

/*
	Merge sort function that uses threads. Two original threads were used, one for the left half and one for the right. Each thread keeps splitting and creating more left and right threads for both sides. It then merges and exits the thread. Print statements are output to document every time a thread is created, every time they merge, and every time they are exitted.
*/

void* merge_sort(void* param){

	struct index* sort = (struct index*) param;
	int low = sort->low;
	int high = sort->high;
	int create1;
	int create2;
	if (low==high){
    	pthread_exit(0);
	}
	pthread_t thread1,thread2;

	struct index sort1,sort2;

	int mid = (low+high)/2;

	sort1.low = low;    
	sort1.high = mid;
	sort2.low = mid+1;  
	sort2.high = high;

	create1 = pthread_create(&thread1,NULL,merge_sort,(void*) &sort1);
	printf("Creating thread for left half of list index %d-%d\n",sort1.low,sort1.high);

	if (create1>0){
    	printf("failed to create new thread 1\n");
	}
	printf("Merging for index %d-%d\n",sort1.low,sort1.high);
	pthread_join(thread1,NULL);


	create2 = pthread_create(&thread2,NULL,merge_sort,(void*) &sort2);
	printf("Creating thread for right half of list index %d-%d\n",sort2.low,sort2.high);

	if (create2>0){
    	printf("failed to create new thread 2\n");
	}

	printf("Merging for index %d-%d\n",sort2.low,sort2.high);
	pthread_join(thread2,NULL);
	int k = low;
	int i = low;
	int j = mid+1;

	while (i<=mid && j<=high){
    	if (arrayNum[i] < arrayNum[j]){
        	sorter[k++] = arrayNum[i++];
    	} else{
        	sorter[k++] = arrayNum[j++];
    	}
	}
	for (; i<=mid ; i++){
    	sorter[k++] = arrayNum[i];
	}
	for (; j<=high ; j++){
    	sorter[k++] = arrayNum[j];
	}
	for (i= low ; i <= high ;i++){
    	arrayNum[i] = sorter[i];
	}
	printf("Exiting thread for index %d-%d\n",sort1.low,sort1.high);
	printf("Exiting thread for index %d-%d\n",sort2.low,sort2.high);
	pthread_exit(0);
}
int main(void) {
	readin("input.txt");

	struct index start;
	start.low = 0;    
	start.high = run-1;
	pthread_t start_thread;


	pthread_create(&start_thread,NULL,merge_sort,&start);
	pthread_join(start_thread,NULL);

	printf("The List Sorted:\n");
	print(arrayNum);
	return 0;
}

