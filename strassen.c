#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8

int(*shuffled_array()) [SIZE];
void print_array(int (*arr)[SIZE]);
int(*strassen(int (*arr)[SIZE])) [SIZE];
int main(){
	srand(time(NULL));
	int (*arr1)[SIZE] = shuffled_array();
	int (*arr2)[SIZE] = shuffled_array();
	
	print_array(arr1);
	printf("hello world\n");
	print_array(arr2);
	return 0;
}
int(*shuffled_array()) [SIZE] {
	int cnt,cnt2;
	int (*pr)[SIZE] = calloc(sizeof(int),SIZE*SIZE);
	for(cnt=0;cnt<SIZE;cnt++) for(cnt2=0;cnt2<SIZE;cnt2++) pr[cnt][cnt2] = rand()%1000;
	return pr;
}

void print_array(int (*arr)[SIZE]) {
	int cnt,cnt2;
	for(cnt=0;cnt<SIZE;cnt++) {
		for(cnt2=0;cnt2<SIZE;cnt2++) printf("%3d | ",arr[cnt][cnt2]);
		printf("\n");
	}
	printf("\n");
	free(arr);
}
int(*strassen(int(*arr1) [SIZE],int(*arr2) [SIZE]) [SIZE] {
	int M1,M2,M3,M4,M5,M6,M7;
	int (*result) [SIZE] = calloc(sizeof(int),SIZE*SIZE);
	
}
