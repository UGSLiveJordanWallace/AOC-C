#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

	while (fgets(line, read_amount, file)) {
		for (int i = 0; i < strlen(line); i++) {
			if (!isdigit(line[i]) && line[i] != '.' && line[i] != '\n') {
				for (int j = 0; j < strlen(prev); j++) {
					if ((prev[j] != '.' && prev[j] != '\n') && (j == 0 || prev[j - 1] == '.') && isdigit(prev[j])) {
						int num;
						sscanf_s(&prev[j], "%d", &num);
						int ds = get_num_digits(num);
						if (j >= i - 1 && j <= i + 1) {
							printf("Prev Num: %d\n", num);
							total += num;
							continue;
						}
						if (j + ds - 1 >= i - 1 && j + ds - 1 <= i + 1) {
							printf("Prev Num: %d\n", num);
							total += num;
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
					printf("Num: %d\n", num);
					total += num;
					continue;
				}
				if (!isdigit(line[(i + ds)]) && line[(i + ds)] != '.' && line[(i + ds)] != '\n') {
					printf("Num: %d\n", num);
					total += num;
					continue;
				}

				for (int j = 0; j < strlen(prev); j++) {
					if (!isdigit(prev[j]) && prev[j] != '.' && prev[j] != '\n') {
						if (i >= j - 1 && i <= j + 1) {
							printf("Num: %d\n", num);
							total += num;
							continue;
						}
						if (i + ds - 1 >= j - 1 && i + ds - 1 <= j + 1) {
							printf("Num: %d\n", num);
							total += num;
							continue;
						}
					}
				}
			}
		}

		memcpy(prev, line, read_amount);
	}

	printf("Total: %d\n", total);

	return 0;
}
