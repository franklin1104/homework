#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "genData.h"

typedef struct ThreadData {
	int start, end;
	int* array;
	int length;
}ThreadData;

void p_mergeSort(void* data);
void merge(int array[], int length, int left, int m, int right);
void mergeSort(int array[], int length, int start, int end);
int pthCount = 0;
int pthSum = 4;

void p_mergeSort(void* data)
{
	struct ThreadData* data_ptr = (struct ThreadData*)data;
	mergeSort(data_ptr->array, data_ptr->length, data_ptr->start, data_ptr->end);
}

void merge(int array[], int length, int left, int m, int right)
{
	int* temp = (int*)malloc(sizeof(int) * length);
	int i, j, k;
	i = left;
	j = m + 1;
	k = 0;

	for (; i <= m && j <= right;) {
		if (array[i] < array[j]) {
			temp[k++] = array[i++];
		}
		else {
			temp[k++] = array[j++];
		}
	}
	while (i <= m) {
		temp[k++] = array[i++];
	}
	while (j <= m) {
		temp[k++] = array[j++];
	}
	k = 0;
	for (i = left; i <= right; i++) {
		array[i] = temp[k++];
	}
	free(temp);
}

void mergeSort(int array[], int length, int start, int end)
{
	if (start < end) {
		int i;
		pthread_t tid1, tid2;
		ThreadData data_left, data_right;
		i = (end + start) / 2;

		data_left.array = array;
		data_left.length = length;
		data_left.start = start;
		data_left.end = i;

		data_right.array = array;
		data_right.length = length;
		data_right.start = i + 1;
		data_right.end = end;

		pthread_create(&tid1, NULL, (void*)p_mergeSort, (void*)&data_left);
		pthread_create(&tid2, NULL, (void*)p_mergeSort, (void*)&data_right);
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
		merge(array, length, start, i, end);
	}
}

int main() {
	pthSum = 16;
	const char* fileName = "4M";
	const int dataCount = 5;
	const int dataSize = dataCount * sizeof(int);
	pthread_t tid;
	int ret;
	
	int* buffer = (int*)malloc(dataSize);
	
	ThreadData data;
	genData(fileName, dataCount);

	FILE* file;
	fopen_s(&file, fileName, "rb");
	fseek(file, 0L, SEEK_SET);
	fread(buffer, 4, dataCount, file);
	for(int i=0;i<5;i++){
		printf("%d ", buffer[i]);
	}
	printf("\n");

	data.array = buffer;
	data.start = 0;
	data.end = dataCount -1;
	data.length = dataCount;

	ret = pthread_create(&tid, NULL, p_mergeSort, &data);
	if (ret) {
		printf("Thread Create Error\n");
		exit(0);
	}

	pthread_join(tid, NULL);
	for(int i=0;i<5;i++){
		printf("%d ", buffer[i]);
	}
	printf("finish\n");
	return 0;
}

