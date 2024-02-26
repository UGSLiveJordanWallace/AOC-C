#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* nums[9] = {
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
};

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
		int f_idx = 1000;
		int l_idx = -1;

		int fnum = 0;
		int lnum = 0;

		for (int i = 0; i < strlen(line); i++) {
			if (!isdigit(line[i])) {
				continue;
			}
			if (fnum == 0) {
				const char c = line[i];
				sscanf_s(&c, "%d", &fnum);
				sscanf_s(&line[i], "%d", &lnum);
				f_idx = i;
				l_idx = i;
			} else {
				sscanf_s(&line[i], "%d", &lnum);
				l_idx = i;
			}
		}

		for (int i = 0; i < 9; i++) {
			char *p = strstr(line, nums[i]);
			int offset = strlen(nums[i]);
			int index = 0;

			while (p != NULL) {
				index = p - line;

				if (f_idx > index) {
					f_idx = index;
					fnum = i + 1;
				}
				if (l_idx < index) {
					l_idx = index;
					lnum = i + 1;
				}

				p = strstr((line + offset + index), nums[i]);
			}
		}


		printf("%d - %d\n", fnum, lnum);
		total += (fnum * 10) + lnum;
	}
	printf("%d\n", total);
	return 0;
}
