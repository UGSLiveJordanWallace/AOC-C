#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
	Five = 6,
	Four = 5,
	Full = 4,
	Three = 3,
	TwoP = 2,
	OneP = 1,
	High = 0,
} HandType;

typedef struct {
	int size;
	char* cards;
	HandType type;
	int bet;
} Hand;
typedef struct {
	int size;
	Hand* hands;
} HandList;

typedef struct Node {
	char key;
	int value;
	struct Node *next;
} Node;
typedef struct {
	int size;
	int r_size;
	Node* nodes;
} HashMap;

int hash(int size, char key) {
	int ascii = key;
	return ascii % size;
}
void map_init(HashMap* hmap) {
	hmap->nodes = (Node*)calloc(hmap->size, sizeof(Node));

	for (int i = 0; i < hmap->size; i++) {
		hmap->nodes[i].next = NULL;
		hmap->nodes[i].value = -1;
		hmap->nodes[i].key = '\0';
	}

	hmap->r_size = 0;
}
int map_get(HashMap* hmap, char key) {
	int index = hash(hmap->size, key);

	if (hmap->nodes[index].key == key) {
		return hmap->nodes[index].value;
	}

	Node* curr = hmap->nodes[index].next;
	while (curr != NULL) {
		if (curr->key == key) {
			return curr->value;
		}
		curr = curr->next;
	}

	return -1;
}
Node* insert(Node* n, char key, int value) {
	if (n == NULL) {
		n = (Node*)calloc(1, sizeof(Node));
		memmove_s(&n->key, 1, &key, 1);
		n->value = value;
		n->next = NULL;
		return n;
	}

	n->next = insert(n->next, key, value);
	return n;
}
void map_insert(HashMap* hmap, char key, int value) {
	int index = hash(hmap->size, key);

	if (hmap->nodes[index].value != -1) {
		hmap->nodes[index].next = insert(hmap->nodes[index].next, key, value);
		hmap->r_size++;
		return;
	}

	memmove_s(&hmap->nodes[index].key, 1, &key, 1);
	hmap->nodes[index].value = value;
	hmap->nodes[index].next = NULL;
	hmap->r_size++;
}
void map_update(HashMap* hmap, char key, int value) {
	int index = hash(hmap->size, key);

	if (hmap->nodes[index].key == key) {
		hmap->nodes[index].value = value;
	}

	Node* curr = hmap->nodes[index].next;
	while (curr != NULL) {
		if (curr->key == key) {
			curr->value = value;
		}
		curr = curr->next;
	}
}
void print_map(HashMap* hmap) {
	for (int i = 0; i < hmap->size; i++) {
		printf("{%c: %d} -> ", hmap->nodes[i].key, hmap->nodes[i].value);
		Node* curr = hmap->nodes[i].next;
		while (curr != NULL) {
			printf("{%c, %d} -> ", curr->key, curr->value);
			curr = curr->next;
		}
		printf("\n");
	}
}
void clear_nodes(Node* n) {
	if (n == NULL) {
		return;
	}

	clear_nodes(n->next);
	n = NULL;
}
void free_map(HashMap* hmap) {
	for (int i = 0; i < hmap->size; i++) {
		clear_nodes(hmap->nodes[i].next);
	}
	
	free(hmap->nodes);
}

HandType calc_hand_type(HashMap* hmap, char* hand) {
	switch (hmap->r_size) {
		case 1:
			return Five;
		case 2:
			for (int i = 0; hand[i] != '\0'; i++) {
				if (hand[i] == 'J') {
					return Five;
				}
			}
			
			for (int i = 0; i < hmap->size; i++) {
				if (hmap->nodes[i].value == 4 || hmap->nodes[i].value == 1) {
					return Four;
				}
				Node* curr = hmap->nodes[i].next;
				while (curr != NULL) {
					if (curr->value == 4 || curr->value == 1) {
						return Four;
					}
					curr = curr->next;
				}
			}

			return Full;
		case 3:
			int j_count = 0;
			for (int i = 0; hand[i] != '\0'; i++) {
				if (hand[i] == 'J') {
					j_count++;
				}
			}

			for (int i = 0; i < hmap->size; i++) {
				if (hmap->nodes[i].value == 3) {
					if (j_count > 0) {
						return Four;
					}
					return Three;
				}
				Node* curr = hmap->nodes[i].next;
				while (curr != NULL) {
					if (curr->value == 3) {
						if (j_count > 0) {
							return Four;
						}
						return Three;
					}
					curr = curr->next;
				}
			}

			if (j_count >= 2) {
				return Four;
			}
			else if (j_count == 1) {
				return Full;
			}

			return TwoP;
		case 4:
			for (int i = 0; hand[i] != '\0'; i++) {
				if (hand[i] == 'J') {
					return Three;
				}
			}
			return OneP;
		default:
			for (int i = 0; hand[i] != '\0'; i++) {
				if (hand[i] == 'J') {
					return OneP;
				}
			}
			return High;
	}
}

void merge(HandList* h_list, HashMap card_strengths, int l, int r, int m) {
	int arr1_l = (m - l) + 1;
	int arr2_l = (r - m);
	Hand arr1_hands[arr1_l];
	Hand arr2_hands[arr2_l];

	for (int i = 0; i < arr1_l; i++) {
		arr1_hands[i] = h_list->hands[l + i];
	}
	for (int i = 0; i < arr2_l; i++) {
		arr2_hands[i] = h_list->hands[m + 1 + i];
	}

	int i = 0, j = 0, k = l;
	while (i < arr1_l && j < arr2_l) {
		if (arr1_hands[i].type < arr2_hands[j].type) {
			h_list->hands[k] = arr1_hands[i];
			i++;
		} else if (arr1_hands[i].type == arr2_hands[j].type) {
			for (int l = 0; l < arr1_hands[i].size; l++) {
				if (arr1_hands[i].cards[l] == arr2_hands[j].cards[l]) {
					continue;
				}
//				printf("Collision: %s %s at %c & %c | ", arr1_hands[i].cards, arr2_hands[j].cards, arr1_hands[i].cards[l], arr2_hands[j].cards[l]);
				int arr1_strength = map_get(&card_strengths, arr1_hands[i].cards[l]);
				int arr2_strength = map_get(&card_strengths, arr2_hands[j].cards[l]);
				if (arr1_strength < arr2_strength) {
//					printf("Collision Resolved: %s\n", arr1_hands[i].cards);
					h_list->hands[k] = arr1_hands[i];
					i++;
				} else {
//					printf("Collision Resolved: %s\n", arr2_hands[i].cards);
					h_list->hands[k] = arr2_hands[j];
					j++;
				}
				break;
			}
		} else {
			h_list->hands[k] = arr2_hands[j];
			j++;
		}
		k++;
	}

	while (i < arr1_l) {
		h_list->hands[k] = arr1_hands[i];
		i++;
		k++;
	}
	while (j < arr2_l) {
		h_list->hands[k] = arr2_hands[j];
		j++;
		k++;
	}
}

void merge_sort(HandList* h_list, HashMap card_strengths, int l, int r) {
	if (l >= r) {
		return;
	}
	int m = (l + r) / 2;
	merge_sort(h_list, card_strengths, l, m);
	merge_sort(h_list, card_strengths, m + 1, r);
	merge(h_list, card_strengths, l, r, m);
}

int main(int args, char* argc[]) {
	if (args != 2) {
		return -1;
	}

	FILE* file;

	fopen_s(&file, argc[1], "r");

	char line[50];
	uint64_t total = 0;
	Hand hand;

	HashMap card_strengths;
	card_strengths.size = 11;
	card_strengths.r_size = 0;
	card_strengths.nodes = (Node*)calloc(card_strengths.size, sizeof(Node));
	map_init(&card_strengths);
	map_insert(&card_strengths, 'A', 12);
	map_insert(&card_strengths, 'K', 11);
	map_insert(&card_strengths, 'Q', 10);
	map_insert(&card_strengths, 'T', 9);
	map_insert(&card_strengths, '9', 8);
	map_insert(&card_strengths, '8', 7);
	map_insert(&card_strengths, '7', 6);
	map_insert(&card_strengths, '6', 5);
	map_insert(&card_strengths, '5', 4);
	map_insert(&card_strengths, '4', 3);
	map_insert(&card_strengths, '3', 2);
	map_insert(&card_strengths, '2', 1);
	map_insert(&card_strengths, 'J', 0);

	HashMap repeated_cards;
	repeated_cards.size = 5;
	repeated_cards.r_size = 0;
	repeated_cards.nodes = (Node*)calloc(repeated_cards.size, sizeof(Node));

	HandList h_list;
	h_list.size = 0;
	h_list.hands = (Hand*)calloc(h_list.size, sizeof(Hand));

	while (fgets(line, 50, file)) {
		hand.bet = 0;
		hand.size = 0;
		hand.cards = (char*)calloc(hand.size, sizeof(char));
		int card_mode = 1;
		map_init(&repeated_cards);
		repeated_cards.r_size = 0;

		for (int i = 0; i < strlen(line); i++) {
			 if (line[i] == 32) {
				 card_mode = 0;
				 continue;
			 }

			 if (i != 0 && isdigit(line[i]) && !isdigit(line[i - 1]) && !card_mode) {
				 int num;
				 sscanf_s(&line[i], "%d", &num);
				 hand.bet = num;
			 }

			 if (card_mode) {
				 hand.size++;
				 hand.cards = (char*)realloc(hand.cards, hand.size * sizeof(char));
				 hand.cards[hand.size - 1] = line[i];

				 int value = map_get(&repeated_cards, line[i]);
				 if (value == -1) {
					 map_insert(&repeated_cards, line[i], 1);
				 } else {
					 map_update(&repeated_cards, line[i], value + 1);
				 }
			 }

			 if (i == strlen(line) - 1) {
				 hand.type = calc_hand_type(&repeated_cards, hand.cards);

//				 printf("Hand: %s | Hand Type: %d\n", hand.cards, hand.type);

				 h_list.size++;
				 h_list.hands = (Hand*)realloc(h_list.hands, h_list.size * sizeof(Hand));
				 h_list.hands[h_list.size - 1].type = hand.type;
				 h_list.hands[h_list.size - 1].size = hand.size;
				 h_list.hands[h_list.size - 1].cards = hand.cards;
				 h_list.hands[h_list.size - 1].bet = hand.bet;
			 }
		}

		free_map(&repeated_cards);
	}

	merge_sort(&h_list, card_strengths, 0, h_list.size - 1);

	for (int i = 0; i < h_list.size; i++) {
//		printf("{%s %d (%d $%d)}\n", h_list.hands[i].cards, h_list.hands[i].type, (i + 1), h_list.hands[i].bet);
		total += (i + 1) * h_list.hands[i].bet;
	}
	
	printf("Total: %llu", total);

	free_map(&card_strengths);
	return 0;
}
