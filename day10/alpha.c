#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int x;
	int y;
} Vector;

typedef struct {
	Vector* v;
} Directions;

void pick_initial(char d, int r_idx, int c_idx, int* r_origin, int* c_origin, Vector *v) {
	switch (d) {
		case '|':
			if (r_idx > *r_origin && c_idx == *c_origin) {
				v->y = 1;
				v->x = 0;
				*r_origin += 1;
			} else if (r_idx < *r_origin && c_idx == *c_origin) {
				v->y = -1;
				v->x = 0;
				*r_origin += -1;
			}
			break;
		case '-':
			if (r_idx == *r_origin && c_idx > *c_origin) {
				v->x = 1;
				v->y = 0;
				*c_origin += 1;
			} else if (r_idx == *r_origin && c_idx < *c_origin) {
				v->x = -1;
				v->y = 0;
				*c_origin += -1;
			}
			break;
		case 'L':
			if (*r_origin < r_idx && *c_origin == c_idx) {
				v->y = 0;
				v->x = 1;
				*r_origin += 1;
			} else if (*r_origin == r_idx && *c_origin > c_idx) {
				v->y = -1;
				v->x = 0;
				*c_origin += -1;
			}
			break;
		case 'J':
			if (*r_origin < r_idx && *c_origin == c_idx) {
				v->y = 0;
				v->x = -1;
				*r_origin += 1;
			} else if (*r_origin == r_idx && *c_origin < c_idx) {
				v->y = -1;
				v->x = 0;
				*c_origin += 1;
			}
			break;
		case '7':
			if (*r_origin > r_idx && *c_origin == c_idx) {
				v->y = 0;
				v->x = -1;
				*r_origin += -1;
			} else if (*r_origin == r_idx && *c_origin < c_idx) {
				v->y = 1;
				v->x = 0;
				*c_origin += 1;
			}
			break;
		case 'F':
			if (*r_origin > r_idx && *c_origin == c_idx) {
				v->y = 0;
				v->x = 1;
				*r_origin += -1;
			} else if (*r_origin == r_idx && *c_origin > c_idx) {
				v->y = 1;
				v->x = 0;
				*c_origin += -1;
			}
			break;
		default:
			break;
	}
}

int move(char d, Vector *v) {
	int found = 0;

	switch (d) {
		case '|':
			if (v->y == 1 && v->x == 0) {
				v->y = 1;
				v->x = 0;
			} else if (v->y == -1 && v->x == 0) {
				v->y = -1;
				v->x = 0;
			}
			break;
		case '-':
			if (v->y == 0 && v->x == 1) {
				v->x = 1;
				v->y = 0;
			} else if (v->y == 0 && v->x == -1) {
				v->x = -1;
				v->y = 0;
			}
			break;
		case 'L':
			if (v->y == 1 && v->x == 0) {
				v->y = 0;
				v->x = 1;
			} else if (v->y == 0 && v->x == -1) {
				v->y = -1;
				v->x = 0;
			}
			break;
		case 'J':
			if (v->y == 1 && v->x == 0) {
				v->y = 0;
				v->x = -1;
			} else if (v->y == 0 && v->x == 1) {
				v->y = -1;
				v->x = 0;
			}
			break;
		case '7':
			if (v->y == -1 && v->x == 0) {
				v->y = 0;
				v->x = -1;
			} else if (v->y == 0 && v->x == 1) {
				v->y = 1;
				v->x = 0;
			}
			break;
		case 'F':
			if (v->y == -1 && v->x == 0) {
				v->y = 0;
				v->x = 1;
			} else if (v->y == 0 && v->x == -1) {
				v->y = 1;
				v->x = 0;
			}
			break;
		case 'S':
			found  = 1;
			break;
		default:
			break;
	}

	return found;
}

void find(char** map, int m_size, int s_row, int s_col, Directions* ds) {
	Vector v;
	v.x = 0;
	v.y = 0;
	int r_origin = s_row;
	int c_origin = s_col;

	for (int i = 0; i < 4; i++) {
		pick_initial(map[ds->v[i].y + s_row][ds->v[i].x + s_col], ds->v[i].y + s_row, ds->v[i].x + s_col, &r_origin, &c_origin, &v);
		if (v.y != 0 || v.x != 0) {
			printf("(%d, %d) + (%d, %d) -> (%d, %d)\n", s_row, s_col, v.y, v.x, r_origin, c_origin);
			break;
		}
	}

	int found = 0;
	unsigned int steps = 0;
	while (!found) {
		r_origin += v.y;
		c_origin += v.x;
		steps++;
		found = move(map[r_origin][c_origin], &v);
	}
	printf("\n");

	if (steps % 2 == 1) {
		printf("Total Steps: %u\n", (steps / 2) + 1);
	} else {
		printf("Total Steps: %u\n", (steps / 2));
	}
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;
	fopen_s(&file, argc[1], "r");

	char line[300];
	uintmax_t total = 0;

	int m_size = 0;
	char** map = (char**)calloc(m_size, sizeof(char*));

	Vector vup;
	vup.x = 0;
	vup.y = -1;

	Vector vdown;
	vdown.x = 0;
	vdown.y = 1;

	Vector vright;
	vright.x = 1;
	vright.y = 0;

	Vector vleft;
	vleft.x = -1;
	vleft.y = 0;

	Directions ds;
	ds.v = (Vector*)calloc(4, sizeof(Vector));
	ds.v[0] = vup;
	ds.v[1] = vright;
	ds.v[2] = vdown;
	ds.v[3] = vleft;

	int found = 0;
	int row = 0;
	int col = 0;
	while (fgets(line, 300, file)) {
		m_size++;
		map = (char**)realloc(map, m_size * sizeof(char*));
		map[m_size - 1] = (char*)calloc(300, sizeof(char));
		memcpy_s(map[m_size - 1], 300, line, 300); 

		if (found) {
			continue;
		}
		for (int i = 0; i < strlen(line); i++) {
			if (line[i] == 'S') {
				found = 1;
				col = i;
				break;
			}
		}
		if (found) {
			continue;
		}
		row++;
	}

	find(map, m_size, row, col, &ds);

	return 0;
}
