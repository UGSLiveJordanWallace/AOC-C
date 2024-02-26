#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int get_digits(int num) {
	if (num < 1) {
		return 0;
	}

	return 1 + get_digits(num / 10);
}

int calc_num_wins(int time, uint64_t dist_to_beat, int ms) {
	uint64_t num_wins = 0; 
	uint64_t min = 0;

	for (uint64_t i = 0; i <= ms; i++) {
		uint64_t dist = i * (time - i);
		if (dist <= dist_to_beat) {
			continue;
		}
		min = i;
		break;
	}

	uint64_t max = time - min;

	num_wins = max - min + 1;

	return num_wins;
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;

	fopen_s(&file, argc[1], "r");

	char line[200];
	int total = 0;

	int time = 0;
	uint64_t dist = 0;

	int rows = 0;
	while (fgets(line, 200, file)) {
		for (int i = 0; i < strlen(line); i++) {
			if (rows == 0) {
				if (isdigit(line[i]) && !isdigit(line[i - 1])) {
					int num;
					sscanf_s(&line[i], "%d", &num);
					int ds = get_digits(num);
					time = (time * (int)pow(10, ds)) + num;
				}
			} else {
				if (isdigit(line[i]) && !isdigit(line[i - 1])) {
					int num;
					sscanf_s(&line[i], "%d", &num);
					int ds = get_digits(num);
					dist = (dist * (int)pow(10, ds)) + num;
				}
			}
		}

		rows++;
	}

	printf("Total: %d\nTime: %u Dist: %llu\n", calc_num_wins(time, dist, time - 1), time, dist);

	return 0;
}
