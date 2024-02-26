#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node {
	char key[3];
	int conn_size;
	struct Node* connections[2];
	struct Node* next;
} Node;
typedef struct {
	int size;
	Node* nodes;
} Graph;

int hash(int size, char k[3]) {
	int s = 0;
	for (int i = 0; k[i] != 0; i++) {
		s += k[i];
	}
	return s % size;
}
void init(Graph* g, int size) {
	g->size = size;
	g->nodes = (Node*)calloc(size, sizeof(Node));
	for (int i = 0; i < size; i++) {
		memset(g->nodes[i].key, '\0', 1);
		g->nodes[i].next = NULL;
		g->nodes[i].conn_size = 0;
	}
}
Node* get(Graph* g, char key[3]) {
	int	index = hash(g->size, key);

	if (strcmp(g->nodes[index].key, key) == 0) {
		return &g->nodes[index]; 
	}

	Node* n = g->nodes[index].next;
	while (n != NULL) {
		if (strcmp(n->key, key) == 0) {
			return n;
		}
		n = n->next;
	}

	return NULL;
}
Node* insert(Node* n, char key[3]) {
	if (n == NULL) {
		n = (Node*)calloc(1, sizeof(Node));
		n->conn_size = 0;
		n->next = NULL;
		memcpy(n->key, key, 3);
		return n;
	}

	n->next = insert(n->next, key);
	return n;
}
void add_node(Graph* g, char key[3]) {
	int index = hash(g->size, key);

	Node* n = get(g, key);
	if (n != NULL) {
		return;
	}

	if (strlen(g->nodes[index].key) != 0) {
		g->nodes[index].next = insert(g->nodes[index].next, key);
		return;
	}

	memcpy_s(g->nodes[index].key, 3, key, 3);
	g->nodes[index].conn_size = 0;
	g->nodes[index].next = NULL;
}
void connection(Graph* g, char key[3], char conn[3]) {
	Node* connection = get(g, conn);
	if (connection == NULL) {
		add_node(g, conn);
		connection = get(g, conn);
	}

	Node* parent = get(g, key);
	if (parent == NULL) {
		add_node(g, key);
		parent = get(g, key);
	}

	parent->conn_size++;
	parent->connections[parent->conn_size - 1] = connection;
}
void clear_nodes(Node* n) {
	if (n == NULL) {
		return;
	}

	clear_nodes(n->next);
	n = NULL;
}
void print_graph(Graph* g) {
	printf("-----------------------------\nGraph:\n");
	for (int i = 0; i < g->size; i++) {
		printf("Node: %s | Connections: %d\n", g->nodes[i].key, g->nodes[i].conn_size);
		Node* curr = g->nodes[i].next;
		while (curr != NULL) {
			printf("\t| -> Node: %s | Connections: %d\n", curr->key, curr->conn_size);
			curr = curr->next;
		}
	}
}
void print_connections(Graph* g) {
	printf("-----------------------------\nGraph Connections:\n");
	for (int i = 0; i < g->size; i++) {
		printf("Node: %s => ", g->nodes[i].key);
		for (int j = 0; j < g->nodes[i].conn_size; j++) {
			printf("%s | ", g->nodes[i].connections[j]->key);
		}
		printf("\n");
		Node* curr = g->nodes[i].next;
		while (curr != NULL) {
			printf("\t| -> Node %s => ", curr->key);
			for (int l = 0; l < curr->conn_size; l++) {
				printf("%s | ", curr->connections[l]->key);
			}
			printf("\n");
			curr = curr->next;
		}
	}
}
void free_graph(Graph* g) {
	for (int i = 0; i < g->size; i++) {
		clear_nodes(g->nodes[i].next);
	}
	
	free(g->nodes);
}
uintmax_t gcd(uintmax_t a, uintmax_t b) { 
  if (b == 0) 
    return a; 
  return gcd(b, a % b); 
} 
uintmax_t lcm(uintmax_t a, uintmax_t b) { 
    return (a / gcd(a, b)) * b; 
} 
void find(Node** nodes, int n_starting_positions, char* instructions, int size) {
	printf("Given Keys: \n");
	for (int i = 0; i < n_starting_positions; i++) {
		printf("\t%s -> (Left: %s, Right: %s)\n", nodes[i]->key, nodes[i]->connections[0]->key, nodes[i]->connections[1]->key);
	}
	printf("\n");

	uintmax_t n = 1;
	for (int j = 0; j < n_starting_positions; j++) {
		uintmax_t steps = 0;
		int i = 0;
		while (1) {
			if (nodes[j]->key[2] == 'Z') {
				break;
			}
			if (instructions[i] == 'L') {
				steps += 1;
				nodes[j] = nodes[j]->connections[0];
				i++;
			} else if (instructions[i] == 'R') {
				steps += 1;
				nodes[j] = nodes[j]->connections[1];
				i++;
			}
			if (i == size) {
				i = 0;
			} 
		}
		n = lcm(n, steps);
	}

	printf("Total steps: %llu\n", n);

	printf("After: \n");
	for (int i = 0; i < n_starting_positions; i++) {
		printf("\t%s -> (Left: %s, Right: %s)\n", nodes[i]->key, nodes[i]->connections[0]->key, nodes[i]->connections[1]->key);
	}
	printf("\n");
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* f;
	fopen_s(&f, argc[1], "r");

	char line[1000];
	unsigned int steps = 0;
	
	int inst_size = 0;
	char* instructions;

	Graph graph;
	init(&graph, 23);

	int n_starting_points = 0;
	Node** starting_nodes = (Node**)calloc(n_starting_points, sizeof(Node*));

	int rows = 0;
	while(fgets(line, 1000, f)) {
		if (rows == 0) {
			inst_size = 0;
			instructions = (char*)calloc(inst_size, sizeof(char));
			for (int i = 0; line[i] != '\n'; i++) {
				if (line[i] == 32) {
					continue;
				}
				inst_size++;
				instructions = (char*)realloc(instructions, inst_size * sizeof(char));
				instructions[inst_size - 1] = line[i];
			}
			rows++;
			continue;
		}

		if (rows == 1) {
			rows++;
			continue;
		}

		char node[4]; 
		for (int i = 0; i < 3; i++) {
			node[i] = line[i];
		}
		node[3] = '\0';
		add_node(&graph, node);
		Node* n = get(&graph, node);
		if (n->key[2] == 'A') {
			n_starting_points++;
			starting_nodes[n_starting_points - 1] = n;
		}
		
		char link1[4];
		char link2[4];
		for (int i = 0; i < 3; i++) {
			link1[i] = line[7 + i];
			link2[i] = line[12 + i];
		}
		link1[3] = '\0';
		link2[3] = '\0';
		add_node(&graph, link1);
		add_node(&graph, link2);

		connection(&graph, node, link1);
		connection(&graph, node, link2);
	}

	find(starting_nodes, n_starting_points, instructions, inst_size);

	free_graph(&graph);
	return 0;
}
