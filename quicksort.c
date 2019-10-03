#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quicksort(int arr[],int start,int finish);
//int partition(int[] arr, int start, int finish);
void swap(int* p,int* q);

int main() {
	srand(time(NULL));
	printf("hi\n");
	int arr[100],cnt;
	for(cnt=0;cnt<100;cnt++) arr[cnt]=rand()%100;
	for(cnt=0;cnt<100;cnt++) {
		if(cnt%20==0) printf("\n");
		printf("%3d ",arr[cnt]);
	}
	quicksort(arr,0,99);
	for(cnt=0;cnt<100;cnt++) {
		if(cnt%20==0) printf("\n");
		printf("%3d ",arr[cnt]);
	}
	return 0;
}

void quicksort(int arr[],int start,int finish) {
	int cnt,mid,pivot;
	if(start<finish) {		
		pivot = arr[start];
		mid = start;
		for(cnt = start+1; cnt<=finish; cnt++) {
			if(arr[cnt]<pivot) {
				swap(arr+(++mid),arr+cnt);
			}
		}
		swap(arr+mid,arr+start);
		quicksort(arr,start,mid-1);
		quicksort(arr,mid+1,finish);
	}
}
void swap(int* p, int* q) {
	int tmp;
	tmp = *p;
	*p = *q;
	*q = tmp;
}
