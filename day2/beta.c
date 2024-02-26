#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* block_c[3] = {
	"blue",
	"red",
	"green",
};

void get_val_id(char* slice, int* block, int* value) {
	for (int i = 0; i < 3; i++) {
		char *p = strstr(slice, block_c[i]);

		if (p != NULL) {
			*block = i;
		}
	}

	for (int i = 0; i < strlen(slice); i++) {
		if (!isdigit(slice[i])) {
			continue;
		}
		sscanf_s(slice, "%d", value);
	}
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE *file;
	fopen_s(&file, argc[1], "r");

	char line[200];
	int total = 0;

	while(fgets(line, 200, file)) {
		int game_id;
		int num_blocks;
		int block_id;

		int g_val = 0;
		int r_val = 0;
		int b_val = 0;

		int l_sep_idx = 0;

		for (int i = 0; i < strlen(line); i++) {
			if (line[i] == ':') {
				int size = i;
				char slice[size];
				for (int j = 0; j < size; j++) {
					slice[j] = line[j];
				}
				slice[size] = '\0';

				for (int i = 0; i < strlen(slice); i++) {
					if (!isdigit(slice[i])) {
						continue;
					}
					sscanf_s(slice, "%*s %d", &game_id);
				}
				
				l_sep_idx = i + 1;
				printf("%d: ", game_id);
			}
			if (line[i] == ',') {
				int size = (i - l_sep_idx) + 1;
				char slice[size];
				for (int j = 0; j < size - 1; j++) {
					slice[j] = line[l_sep_idx + j];
				}
				slice[size] = '\0';

				int block;
				int value;
				get_val_id(slice, &block, &value);
				printf("%d - %d | ", value, block);

				// red
				if (block == 1 && r_val < value) {
					r_val = value;
				}
				// green
				if (block == 2 && g_val < value)  {
					g_val = value;
				}
				// blue
				if (block == 0 && b_val < value) {
					b_val = value;
				}

				l_sep_idx = i + 1;
			}
			if (line[i] == ';') {
				int size = (i - l_sep_idx) + 1;
				char slice[size];
				for (int j = 0; j < size - 1; j++) {
					slice[j] = line[l_sep_idx + j];
				}
				slice[size] = '\0';

				int block;
				int value;
				get_val_id(slice, &block, &value);
				printf("%d - %d | ", value, block);

				// red
				if (block == 1 && r_val < value) {
					r_val = value;
				}
				// green
				if (block == 2 && g_val < value)  {
					g_val = value;
				}
				// blue
				if (block == 0 && b_val < value) {
					b_val = value;
				}

				l_sep_idx = i + 1;
			}
			if (i == strlen(line) - 1) {
				int size = (i - l_sep_idx) + 1;
				char slice[size];
				for (int j = 0; j < size - 1; j++) {
					slice[j] = line[l_sep_idx + j];
				}
				slice[size] = '\0';

				int block;
				int value;
				get_val_id(slice, &block, &value);
				printf("%d - %d | ", value, block);

				// red
				if (block == 1 && r_val < value) {
					r_val = value;
				}
				// green
				if (block == 2 && g_val < value)  {
					g_val = value;
				}
				// blue
				if (block == 0 && b_val < value) {
					b_val = value;
				}

				l_sep_idx = i + 1;
			}
		}

		int p = r_val * g_val * b_val;
		total += p;
		printf("| (%d, %d, %d) => Power: %d\n", r_val, g_val, b_val, p);
	}

	printf("%d\n", total);
	return 0;
}
