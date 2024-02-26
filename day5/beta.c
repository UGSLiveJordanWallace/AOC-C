#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <omp.h>

typedef struct {
	int size;
	uint64_t* s;
	uint64_t* range;
} seeds_arr;

typedef struct {
	uint64_t dest;
	uint64_t source;
	uint64_t n;
	int curr_d;
} seed_props;

typedef struct {
	int size;
	seed_props* props;
} seed_props_arr;

char* destinations[8] = {
	"seed",
	"soil",
	"fertilizer",
	"water",
	"light",
	"temperature",
	"humidity",
	"location",
};

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;

	fopen_s(&file, argc[1], "r");
	
	char line[1000];
	int total = 0;

	seeds_arr seeds;
	seeds.size = 0;
	seeds.s = (uint64_t*)calloc(seeds.size, sizeof(uint64_t));
	seeds.range = (uint64_t*)calloc(seeds.size, sizeof(uint64_t));

	seed_props props;
	seed_props_arr props_arr;
	props_arr.size = 0;
	props_arr.props = (seed_props*)calloc(props_arr.size, sizeof(seed_props));

	int rows = 0;
	while (fgets(line, 1000, file)) {
		props.dest = -1;
		props.source = -1;
		props.n = -1;

		uint64_t fnum = 0;

		for (int i = 0; i < strlen(line); i++) {
			if (!isdigit(line[i]) && rows != 0) {
				int found = 0;
				for (int j = 0; j < 8; j++) {
					char *p = strstr(line, destinations[j]);
					int offset = strlen(destinations[j]);
					int index = 0;

					if (p != NULL) {
						found = 1;
						props.curr_d = j;
						break;
					}
				}
				
				if (found) {
					break;
				}

				continue;
			}

			if (isdigit(line[i]) && !isdigit(line[i - 1]) && rows != 0) {
				int num = 0;
				sscanf_s(&line[i], "%d", &num);
				if (props.dest == -1) {
					props.dest = num;
				} else if (props.source == -1) {
					props.source = num;
				} else {
					props.n = num;
				}
			}

			if (isdigit(line[i]) && !isdigit(line[i - 1]) && rows == 0) {
				uint64_t num = 0;
				sscanf_s(&line[i], "%d", &num);

				if (fnum == 0) {
					fnum = num;
				} else {
					seeds.size++;
					seeds.s = (uint64_t*)realloc(seeds.s, seeds.size * sizeof(uint64_t));
					seeds.s[seeds.size - 1] = fnum;
					seeds.range = (uint64_t*)realloc(seeds.range, seeds.size * sizeof(uint64_t));
					seeds.range[seeds.size - 1] = num;

					fnum = 0;
				}
			}
		}

		if (props.dest != -1 || props.source != -1 || props.n != -1) {
			props_arr.size++;
			props_arr.props = (seed_props*)realloc(props_arr.props, props_arr.size * sizeof(seed_props));
			props_arr.props[props_arr.size - 1] = props;
		}

		rows++;
	}

	uint64_t lnum = -1;
	int thread_id, nloops;
#pragma omp parallel private(thread_id, nloops)
	{
		nloops = 0;
#pragma omp for
		for (int i = 0; i < seeds.size; i++) {
			nloops++;
			for (int j = 0; j < seeds.range[i]; j++) {
				uint64_t seed = seeds.s[i] + j;
				int prev = -1;

				for (int l = 0; l < props_arr.size; l++) {
					if (props_arr.props[l].curr_d == prev) {
						continue;
					}
					if (seed >= props_arr.props[l].source && seed <= props_arr.props[l].source + props_arr.props[l].n - 1) {
						seed = (seed - props_arr.props[l].source) + props_arr.props[l].dest;
						prev = props_arr.props[l].curr_d;
					}
				}

#pragma omp critical 
				{
					if (lnum == -1) {
						lnum = seed;
					}
					if (lnum > seed) {
						lnum = seed;
					}
				}
			}
		}

		thread_id = omp_get_thread_num();
	}
	printf("Lowest Location: %llu", lnum);

	free(seeds.s);
	free(seeds.range);
	free(props_arr.props);
}
