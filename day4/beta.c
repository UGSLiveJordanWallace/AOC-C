#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int size;
	int* n;
} nums;

typedef struct {
	int size;
	int instances;
	int matches;
	int card;
} card;

typedef struct {
	int size;
	card* c;
} cards_arr;

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;

	fopen_s(&file, argc[1], "r");

	char line[500];
	int total = 0;
	cards_arr cards;
	cards.size = 0;
	cards.c = (card*)calloc(1, sizeof(card));

	while (fgets(line, 500, file)) {
		int slc_idx = 0;
		int start_idx = 0;
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

			if (start_idx == 0) {
				if (isdigit(line[i]) && !isdigit(line[i - 1])) {
					int num;
					sscanf_s(&line[i], "%d", &num);
					cards.size++;
					cards.c = (card*)realloc(cards.c, cards.size * sizeof(card));
					cards.c[cards.size - 1].card = num;
					cards.c[cards.size - 1].instances = 1;
					cards.c[cards.size - 1].matches = 0;
				}
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
//				printf("%d | ", num);

				for (int l = 0; l < n.size; l++) {
					if (n.n[l] == num) {
						cards.c[cards.size - 1].matches++;
					}
				}
			}
		}


		free(n.n);
		printf("\n");
	}

	for (int i = 0; i < cards.size; i++) {
		for (int j = i + 1; j < (i + cards.c[i].matches + 1); j++) {
			cards.c[j].instances += cards.c[i].instances;
		}
		printf("Card: %d\n", cards.c[i].card);
		printf("Matches: %d\n", cards.c[i].matches);
		printf("Instances: %d\n", cards.c[i].instances);
		total += cards.c[i].instances;
	}

	printf("%d\n", total);

	free(cards.c);
	return 0;
}
