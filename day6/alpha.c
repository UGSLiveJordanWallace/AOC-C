#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int size;
	int* times;
	int* dists;
} Races;

int calc_num_wins(int time, int dist_to_beat, int ms) {
	if (ms <= 0) {
		return 0;
	}

	int dist = ms * (time - ms);
	if (dist <= dist_to_beat) {
		return calc_num_wins(time, dist_to_beat, ms - 1);
	}

	return 1 + calc_num_wins(time, dist_to_beat, ms - 1);
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;

	fopen_s(&file, argc[1], "r");

	char line[200];
	int total = 0;

	Races races;
	races.size = 0;
	races.times = (int*)calloc(races.size, races.size * sizeof(int));
	races.dists = (int*)calloc(races.size, races.size * sizeof(int));

	int rows = 0;
	int times_size = 0;
	int dists_size = 0;
	while (fgets(line, 200, file)) {

		for (int i = 0; i < strlen(line); i++) {
			if (rows == 0) {
				if (isdigit(line[i]) && !isdigit(line[i - 1])) {
					int num;
					sscanf_s(&line[i], "%d", &num);

					times_size++;
					races.times = (int*)realloc(races.times, times_size * sizeof(int));
					races.times[times_size - 1] = num;
				}
			} else {
				if (isdigit(line[i]) && !isdigit(line[i - 1])) {
					int num;
					sscanf_s(&line[i], "%d", &num);

					dists_size++;
					races.dists = (int*)realloc(races.dists, dists_size * sizeof(int));
					races.dists[dists_size - 1] = num;
				}
			}
		}

		rows++;
	}

	if (times_size != dists_size) {
		printf("Times and Dists Off!");
		return -1;
	}

	races.size = times_size;
	for (int i = 0; i < races.size; i++) {
		if (total == 0) {
			total = calc_num_wins(races.times[i], races.dists[i], races.times[i] - 1);
			continue;
		}
		total *= calc_num_wins(races.times[i], races.dists[i], races.times[i] - 1);
	}

	printf("Total: %d\n", total);

	free(races.times);
	free(races.dists);
	return 0;
}
