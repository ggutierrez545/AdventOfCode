#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"

typedef struct antenna {
  char frequency;
  int x;
  int y;
  struct antenna *next;
} antenna;

void checkAntinode(struct hashtable *unique_spots, int x, int y, int *num_unique_spots, int row, int col) {
  if ((0 <= x && x < row) && (0 <= y && y < col)) {
    char *antinode = malloc(sizeof(char)*25);
    sprintf(antinode, "(%i,%i)", x, y);
    struct item *it = hash_table_lookup(unique_spots, antinode);
    if (it == NULL) {
      it = malloc(sizeof(struct item));
      strcpy(it->key, antinode);
      hash_table_insert(unique_spots, it);
      (*num_unique_spots)++;
    }
  }
}

int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  // Initial read
  struct antenna **antenna_array = calloc(1000, sizeof(struct antenna*));
  struct hashtable *ht = create_hash_table(1000);
  int row = 0;
  int col;
  int antenna_array_idx = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    for (col = 0; col < strlen(read_buffer); col++) {
      char *key = malloc(sizeof(char));
      strncpy(key, &read_buffer[col], 1);
      if (*key == '.' || *key == '\n') continue;
      struct antenna *ant = calloc(1, sizeof(struct antenna));
      ant->frequency = *key;
      ant->x = row;
      ant->y = col;
      struct item *it = hash_table_lookup(ht, key);
      if (it == NULL) {
        it = malloc(sizeof(struct item));
        strcpy(it->key, key);
        it->value = antenna_array_idx;
        hash_table_insert(ht, it);
        antenna_array_idx++;
      }
      struct antenna *tmp = antenna_array[it->value];
      if (tmp == NULL) {
        antenna_array[it->value] = ant;
        continue;
      }
      while (tmp->next != NULL) {
        tmp = tmp->next;
      }
      tmp->next = ant;
    }
    row++;
  }

  //for (int i = 0; i < antenna_array_idx; i++) {
  //  struct antenna *tmp = antenna_array[i];
  //  while (tmp != NULL) {
  //    printf("Antenna at %c(%i,%i)\n", tmp->frequency, tmp->x, tmp->y);
  //    tmp = tmp->next;
  //  }
  //}

  //printf("Dimensions: %i x %i\n", row, col - 1);
  struct hashtable *unique_spots = create_hash_table(1000);
  int num_unique_spots = 0;
  for (int i = 0; i < antenna_array_idx; i++) {
    struct antenna *curr = antenna_array[i];
    while (curr != NULL) {
      checkAntinode(unique_spots, curr->x, curr->y, &num_unique_spots, row, col - 1);
      struct antenna *nex = curr->next;
      while (nex != NULL) {
        checkAntinode(unique_spots, nex->x, nex->y, &num_unique_spots, row, col - 1);

        int dist_x = curr->x - nex->x;
        int dist_y = curr->y - nex->y;

        // Antinodes up
        int antinode1_x, antinode1_y;
        antinode1_x = curr->x + dist_x;
        antinode1_y = curr->y + dist_y;
        while ((0 <= antinode1_x && antinode1_x < row) && (0 <= antinode1_y && antinode1_y < col - 1)) {
          checkAntinode(unique_spots, antinode1_x, antinode1_y, &num_unique_spots, row, col - 1);
          antinode1_x += dist_x;
          antinode1_y += dist_y;
        }

        // Antinodes down
        int antinode2_x, antinode2_y;
        antinode2_x = nex->x - dist_x;
        antinode2_y = nex->y - dist_y;
        while ((0 <= antinode2_x && antinode2_x < row) && (0 <= antinode2_y && antinode2_y < col - 1)) {
          checkAntinode(unique_spots, antinode2_x, antinode2_y, &num_unique_spots, row, col - 1);
          antinode2_x -= dist_x;
          antinode2_y -= dist_y;
        }
        nex = nex->next;
      }
      curr = curr->next;
    }
  }

  //print_table(unique_spots);

  printf("Num unique spots with antinodes: %i\n", num_unique_spots);
  destroy_hash_table(ht);

  fclose(fp);
  return EXIT_SUCCESS;
}