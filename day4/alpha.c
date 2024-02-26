#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int size;
	int* n;
} nums;

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;

	fopen_s(&file, argc[1], "r");

	char line[500];
	int total = 0;

	while (fgets(line, 500, file)) {
		int slc_idx = 0;
		int start_idx = 0;
		int fnum = 0;
		nums n;
		n.size = 0;
		n.n = (int*)calloc(1, sizeof(int));

		for (int i = 0; i < strlen(line); i++) {
			if (line[i] == ':') {
				start_idx = i + 1;
				continue;
			}
			if (line[i] == '|') {
				slc_idx = i;
				continue;
			}

			if (slc_idx == 0) {
				if (isdigit(line[i]) && !isdigit(line[i - 1]) && start_idx != 0) {
					int num;
					sscanf_s(&line[i], "%d", &num);
					n.size++;
					n.n = (int*)realloc(n.n, n.size * sizeof(int));
					n.n[n.size - 1] = num;
				}
				continue;
			}

			if (isdigit(line[i]) && !isdigit(line[i - 1])) {
				int num;
				sscanf_s(&line[i], "%d", &num);
				printf("%d | ", num);

				for (int l = 0; l < n.size; l++) {
					if (n.n[l] == num) {
						printf("Did Match %d\n", n.n[l]);
						if (fnum == 0) {
							fnum = 1;
						} else {
							fnum *= 2;
						}
					}
				}
			}
		}

		total += fnum;

		free(n.n);
		printf("\n");
	}

	printf("%d\n", total);
	
	return 0;
}
