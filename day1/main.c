#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}
	
	FILE *file;

	fopen_s(&file, argc[1], "r");

	if (file == NULL) {
		printf("Error!");
		return -1;
	}

	char line[100];
	int total = 0;
	while (fgets(line, 100, file)) {
		int fnum = 0;
		int lnum = 0;
		for (int i = 0; i < strlen(line); i++) {
			if (!isdigit(line[i])) {
				continue;
			}
			if (fnum == 0) {
				const char n = line[i];
				fnum = atoi(&n);
			} else {
				sscanf_s(&line[i], "%d\n", &lnum);
			}
		}
		if (lnum == 0) {
			fnum *= 11;
			total += fnum;
		} else {
			total += (fnum * 10) + lnum;
		}
		printf("%d - ", fnum);
		printf("%d\n", lnum);
	}
	printf("%d\n", total);
	return 0;
}
