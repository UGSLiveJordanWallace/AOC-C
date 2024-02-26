#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
	int size;
	uint32_t* s;
	int* mc;
} seeds_arr;

typedef struct {
	int dest;
} curr_dest;

typedef struct {
	uint32_t dest;
	uint32_t source;
	int n;
} seed_props;

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
	seeds.s = (uint32_t*)calloc(seeds.size, sizeof(uint32_t));
	seeds.mc = (int*)calloc(seeds.size, sizeof(int));
	seed_props props;
	curr_dest d;
	int l_loc = -1;

	int rows = 0;

	while (fgets(line, 1000, file)) {
		props.dest = -1;
		props.source = -1;
		props.n = -1;

		for (int i = 0; i < strlen(line); i++) {
			if (!isdigit(line[i]) && rows != 0) {
				int found = 0;
				for (int j = 0; j < 8; j++) {
					char *p = strstr(line, destinations[j]);
					int offset = strlen(destinations[j]);
					int index = 0;

					if (p != NULL) {
						d.dest = j;
						for (int l = 0; l < seeds.size; l++) {
							seeds.mc[l] = 0;
						}
						found = 1;
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
				int num = 0;
				sscanf_s(&line[i], "%d", &num);
				seeds.size++;
				seeds.s = (uint32_t*)realloc(seeds.s, seeds.size * sizeof(uint32_t));
				seeds.s[seeds.size - 1] = num;
				seeds.mc = (int*)realloc(seeds.mc, seeds.size * sizeof(int));
				seeds.mc[seeds.size - 1] = 0;
			}
		}
		
		if (props.dest != -1 || props.source != -1 || props.n != -1) {
			for (int i = 0; i < seeds.size; i++) {
				if (seeds.mc[i] == 1) {
					continue;
				}
				if (seeds.s[i] >= props.source && seeds.s[i] <= props.source + props.n - 1) {
					seeds.s[i] = (seeds.s[i] - props.source) + props.dest;
					seeds.mc[i] = 1;
				}
			}
		}

		rows++;
	}

	for (int i = 0; i < seeds.size; i++) {
		if (l_loc == -1) {
			l_loc = seeds.s[i];
		}
		if (d.dest == 6 && l_loc > seeds.s[i]) {
			l_loc = seeds.s[i];
		}
		printf("%u \n", seeds.s[i]);
	}

	printf("Lowest %u\n", l_loc);

}
