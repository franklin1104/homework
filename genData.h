#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

const int SIZE_M = 1024 * 1024;
void genData(const char* fileName, const int size) {
	const int ARR_SIZE = 16 * SIZE_M;
	
	FILE* file;
	if (!fopen_s(&file, fileName, "ab+")) {
		
		srand(time(0));
		if (size < ARR_SIZE) {
			int* arr = (int*)malloc(4 * size);
			for (int i = 0; i < size; i++) {
				arr[i] = rand();
			}
			fwrite(arr, 2, size, file);
			free(arr);
		}
		else {
			int* arr = (int*)malloc(4 * ARR_SIZE);
			const int iters = size / ARR_SIZE;
			for (int i = 0; i < iters; i++) {
				for (int j = 0; j < ARR_SIZE; j++) {
					arr[j] = rand();
				}
				fwrite(arr, 2, ARR_SIZE, file);
			}
			free(arr);
		}
		fclose(file);
	}else{
		printf("open failed\n");
	}
}