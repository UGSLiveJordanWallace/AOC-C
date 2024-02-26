#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int32_t extrapolation_value(int32_t* vals, int size) {
	if (size <= 1) {
		return 0;
	}
	int is_base_case = 1;
	for (int i = 1; i < size; i++) {
		if (vals[i] != 0) {
			is_base_case = 0;
			break;
		}
	}
	if (is_base_case) {
		return 0;
	}

	int32_t diff_arr[size - 1];
	int32_t curr_diff = 0;
	for (int i = 1; i < size; i++) {
		curr_diff = vals[i] - vals[i - 1];
		diff_arr[i - 1] = curr_diff;
//		printf("%d %d => %d | ", vals[i - 1], vals[i], curr_diff);
	}
//	printf("\n");

	return vals[size - 1] + extrapolation_value(diff_arr, size - 1);
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;
	fopen_s(&file, argc[1], "r");

	char line[1000];
	int32_t total = 0;

	while(fgets(line, 1000, file)) {
		int32_t hist_size = 0;
		int32_t* history = (int32_t*)calloc(hist_size, sizeof(unsigned int));

		for (int i = 0; i < strlen(line); i++) {
			if (i == 0) {
				int num;
				sscanf_s(&line[i], "%d", &num);
//				printf("%d ", num);

				hist_size++;
				history = (int32_t*)realloc(history, hist_size * sizeof(int32_t));
				history[hist_size - 1] = num;
			}
			if (line[i] == 32) {
				int num;
				sscanf_s(&line[i + 1], "%d", &num);
//				printf("%d ", num);

				hist_size++;
				history = (int32_t*)realloc(history, hist_size * sizeof(int32_t));
				history[hist_size - 1] = num;
				continue;
			}
		}
//		printf("\n");
//		printf("New Arr: \n");
		unsigned int value = extrapolation_value(history, hist_size);
//		printf("Extrapolated: %u\n", value);
//		printf("---------------------\n");
		total += value;

		free(history);
	}

	printf("Total: %d", total);

	free(file);
	return 0;
}
