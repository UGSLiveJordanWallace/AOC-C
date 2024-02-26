#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int size;
	int* first_nums;
	int* indices;
	int* rows;
} first_nums;

int get_num_digits(int num) {
	if (num < 1) {
		return 0;
	}

	return 1 + get_num_digits(num / 10);
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;
	fopen_s(&file, argc[1], "r");
	
	int read_amount = 500;
	char line[read_amount];
	char prev[read_amount];
	int total = 0;

	memset(prev, '\0', 1);

	first_nums fnums;
	fnums.size = 0;
	fnums.first_nums = (int*)calloc(fnums.size, sizeof(int));
	fnums.indices = (int*)calloc(fnums.size, sizeof(int));
	fnums.rows = (int*)calloc(fnums.size, sizeof(int));

	int rows = 0;
	while (fgets(line, read_amount, file)) {
		rows++;
		for (int i = 0; i < strlen(line); i++) {
			if (!isdigit(line[i]) && line[i] != '.' && line[i] != '\n') {
				for (int j = 0; j < strlen(prev); j++) {
					if ((prev[j] != '.' && prev[j] != '\n') && (j == 0 || prev[j - 1] == '.') && isdigit(prev[j])) {
						int num;
						sscanf_s(&prev[j], "%d", &num);
						int ds = get_num_digits(num);
						if (j >= i - 1 && j <= i + 1) {
							if (line[i] == '*') {
								int isfound = 0;
								for (int l = 0; l < fnums.size; l++) {
									if (rows == fnums.rows[l] && fnums.indices[l] == i) {
										total += fnums.first_nums[l] * num;
										isfound = 1;
										printf("Num: %d Is Gear Ratio with Num: %d Index %d On Row %d\n", num, fnums.first_nums[l], fnums.indices[l], fnums.rows[l]);
									}
								}
								if (isfound != 1) {
									fnums.size++;
									fnums.first_nums = (int*)realloc(fnums.first_nums, fnums.size * sizeof(int));
									fnums.indices = (int*)realloc(fnums.indices, fnums.size * sizeof(int));
									fnums.rows = (int*)realloc(fnums.rows, fnums.size * sizeof(int));
									fnums.first_nums[fnums.size - 1] = num;
									fnums.indices[fnums.size - 1] = i;
									fnums.rows[fnums.size - 1] = rows;
									printf("Prev Num: %d Is Gear with %d\n", num, i);
								}
							} else {
								printf("Prev Num: %d\n", num);
							}
							continue;
						}
						if (j + ds - 1 >= i - 1 && j + ds - 1 <= i + 1) {
							if (line[i] == '*') {
								fnums.size++;
								fnums.first_nums = (int*)realloc(fnums.first_nums, fnums.size * sizeof(int));
								fnums.indices = (int*)realloc(fnums.indices, fnums.size * sizeof(int));
								fnums.rows = (int*)realloc(fnums.rows, fnums.size * sizeof(int));
								fnums.first_nums[fnums.size - 1] = num;
								fnums.indices[fnums.size - 1] = i;
								fnums.rows[fnums.size - 1] = rows;
								printf("Prev Num: %d Is Gear with %d\n", num, i);
							} else {
								printf("Prev Num: %d\n", num);
							}
							continue;
						}
					}
				}
				continue;
			}

			if ((line[i] != '.' && line[i] != '\n') && (i == 0 || !isdigit(line[i - 1]))) {
				int num;
				sscanf_s(&line[i], "%d", &num);
				int ds = get_num_digits(num);

				if (!isdigit(line[i - 1]) && line[i - 1] != '.' && i != 0) {
					if (line[i - 1] == '*') {
						int isfound = 0;
						for (int l = 0; l < fnums.size; l++) {
							if (rows == fnums.rows[l] && fnums.indices[l] == i - 1) {
								total += fnums.first_nums[l] * num;
								isfound = 1;
								printf("Num: %d Is Gear Ratio with Num: %d Index %d On Row %d\n", num, fnums.first_nums[l], fnums.indices[l], fnums.rows[l]);
							}
						}
						if (isfound != 1) {
							fnums.size++;
							fnums.first_nums = (int*)realloc(fnums.first_nums, fnums.size * sizeof(int));
							fnums.indices = (int*)realloc(fnums.indices, fnums.size * sizeof(int));
							fnums.rows = (int*)realloc(fnums.rows, fnums.size * sizeof(int));
							fnums.first_nums[fnums.size - 1] = num;
							fnums.indices[fnums.size - 1] = i - 1;
							fnums.rows[fnums.size - 1] = rows;
						}
					} else {
						printf("Num: %d\n", num);
					}
					continue;
				}
				if (!isdigit(line[(i + ds)]) && line[(i + ds)] != '.' && line[(i + ds)] != '\n') {
					if (line[(i + ds)] == '*') {
						int isfound = 0;
						for (int l = 0; l < fnums.size; l++) {
							if (rows == fnums.rows[l] && fnums.indices[l] == i + ds) {
								total += fnums.first_nums[l] * num;
								isfound = 1;
								printf("Num: %d Is Gear Ratio with Num: %d Index %d On Row %d\n", num, fnums.first_nums[l], fnums.indices[l], fnums.rows[l]);
							}
						}
						if (isfound != 1) {
							fnums.size++;
							fnums.first_nums = (int*)realloc(fnums.first_nums, fnums.size * sizeof(int));
							fnums.indices = (int*)realloc(fnums.indices, fnums.size * sizeof(int));
							fnums.rows = (int*)realloc(fnums.rows, fnums.size * sizeof(int));
							fnums.first_nums[fnums.size - 1] = num;
							fnums.indices[fnums.size - 1] = i + ds;
							fnums.rows[fnums.size - 1] = rows;
						}
					} else {
						printf("Num: %d\n", num);
					}
					continue;
				}

				for (int j = 0; j < strlen(prev); j++) {
					if (!isdigit(prev[j]) && prev[j] != '.' && prev[j] != '\n') {
						if (i >= j - 1 && i <= j + 1) {
							if (prev[j] == '*') {
								int isfound = 0;
								for (int l = 0; l < fnums.size; l++) {
									if (rows - 1 == fnums.rows[l] && fnums.indices[l] == j) {
										total += fnums.first_nums[l] * num;
										isfound = 1;
										printf("Num: %d Is Gear Ratio with Num: %d Index %d On Row %d\n", num, fnums.first_nums[l], fnums.indices[l], fnums.rows[l]);
									}
								}
							} else {
								printf("Num: %d\n", num);
							}
							continue;
						}
						if (i + ds - 1 >= j - 1 && i + ds - 1 <= j + 1) {
							if (prev[j] == '*') {
								int isfound = 0;
								for (int l = 0; l < fnums.size; l++) {
									if (rows - 1 == fnums.rows[l] && fnums.indices[l] == j) {
										total += fnums.first_nums[l] * num;
										isfound = 1;
										printf("Num: %d Is Gear Ratio with Num: %d Index %d On Row %d\n", num, fnums.first_nums[l], fnums.indices[l], fnums.rows[l]);
									}
								}
								if (isfound != 1) {
									fnums.size++;
									fnums.first_nums = (int*)realloc(fnums.first_nums, fnums.size * sizeof(int));
									fnums.indices = (int*)realloc(fnums.indices, fnums.size * sizeof(int));
									fnums.rows = (int*)realloc(fnums.rows, fnums.size * sizeof(int));
									fnums.first_nums[fnums.size - 1] = num;
									fnums.indices[fnums.size - 1] = j;
									fnums.rows[fnums.size - 1] = rows - 1;
								}
							} else {
								printf("Num: %d\n", num);
							}
							continue;
						}
					}
				}
			}
		}

		memcpy(prev, line, read_amount);
	}

	for (int l = 0; l < fnums.size; l++) {
		printf("-----------------------\n%d Index %d On Row %d\n", fnums.first_nums[l], fnums.indices[l], fnums.rows[l]);
	}

	printf("Total: %d\n", total);

	free(fnums.indices);
	free(fnums.first_nums);

	return 0;
}
