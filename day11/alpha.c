#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	int id;
	int og_idx;
	int x;
	int y;
} Point;

typedef struct {
	int size;
	Point* ps;
} Points;

typedef struct {
	Point p1;
	Point p2;
} Pair;

typedef struct {
	int size;
	Pair* pairs;
} Pairs;

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE *file;
	fopen_s(&file, argc[1], "r");

	char line[150];
	uintmax_t total = 0;

	uint64_t m_rows = 0;
	int m_cols = 0;

	Points ps;
	ps.size = 0;
	ps.ps = (Point*)calloc(ps.size, sizeof(Point));

	int size = 0;
	int* empty_cols = (int*)calloc(100, sizeof(int));

	int ids = 0;
	while (fgets(line, 150, file)) {
		m_cols = 0;
		m_rows++;
		int has_point = 0;
		for (int i = 0; i < strlen(line); i++) {
			m_cols++;
			if (line[i] == '#') {
				has_point = 1;
				ids++;
				ps.size++;
				ps.ps = (Point*)realloc(ps.ps, ps.size * sizeof(Point));
				ps.ps[ps.size - 1].id = ids;
				ps.ps[ps.size - 1].y = m_rows;
				ps.ps[ps.size - 1].x = m_cols;
				ps.ps[ps.size - 1].og_idx = i;
				empty_cols[i] = 1;
			}
			if (size != strlen(line)) {
				size = strlen(line) - 1;
				empty_cols = (int*)realloc(empty_cols, size * sizeof(int));
			}
		}
		if (!has_point) {
			// Calc Scale Rows
			m_rows += 1000000 - 1;
		}
		m_cols = strlen(line);
	}

	// Calc Scale Columns
	int scale = 1000000;
	for (int i = 0; i < size; i++) {
		if (empty_cols[i] == 0) {
			for (int l = 0; l < ps.size; l++) {
				if (ps.ps[l].og_idx > i) {
					ps.ps[l].x += scale - 1;
				}
			}
		}
	}

	Pairs pairs;
	pairs.size = 0;
	pairs.pairs = (Pair*)calloc(pairs.size, sizeof(Pair));

	for (int i = 0; i < ps.size; i++) {
		for (int j = i + 1; j < ps.size; j++) {
			pairs.size++;
			pairs.pairs = (Pair*)realloc(pairs.pairs, pairs.size * sizeof(Pair));
			pairs.pairs[pairs.size - 1].p1 = ps.ps[i];
			pairs.pairs[pairs.size - 1].p2 = ps.ps[j];
		}
	}

	for (int i = 0; i < pairs.size; i++) {
//		printf("--------------------------------\n");
//		printf("Pair: #%d\n", i);
//		printf("Point: (%d %d) ID: %d Original Index: %d \n", pairs.pairs[i].p1.x, pairs.pairs[i].p1.y, pairs.pairs[i].p1.id, pairs.pairs[i].p1.og_idx);
//		printf("Point: (%d %d) ID: %d Original Index: %d \n", pairs.pairs[i].p2.x, pairs.pairs[i].p2.y, pairs.pairs[i].p2.id, pairs.pairs[i].p2.og_idx);
		total += _abs64(pairs.pairs[i].p1.x - pairs.pairs[i].p2.x) + _abs64(pairs.pairs[i].p1.y - pairs.pairs[i].p2.y);
	}

	printf("Total: %llu\n", total);

	return 0;
}
