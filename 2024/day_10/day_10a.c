#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/hashtable.h"


typedef struct trailhead {
  int x;
  int y;
  struct trailhead *next;
} trailhead;

void assess_node(struct hashtable *visited, char **map, int *score, int prev_height, int x, int y, int rows, int cols) {
  // Check if node is on map
  if ((x < 0 || x >= rows) || (y < 0 || y >= cols)) return;

  // Verify this node is a gradual increase from the previous one
  char height_char;
  strncpy(&height_char, &map[x][y], 1);
  int height = atoi(&height_char);
  //printf("Node: (%i,%i), Height: %i", x, y, height);
  if (height - prev_height != 1) {
    //printf(" - Failed\n");
    return;
  }
  //printf(" - Passed\n");

  // Add node to visited table
  char *key = malloc(sizeof(char) * 20);
  sprintf(key, "(%i,%i)", x, y);
  struct item *it = hash_table_lookup(visited, key);
  if (it != NULL) return;
  it = malloc(sizeof(struct item));
  strcpy(it->key, key);
  free(key);
  hash_table_insert(visited, it);

  if (height == 9) {
    (*score)++;
    return;
  }

  // Assess node above
  assess_node(visited, map, score, height, x - 1, y, rows, cols);
  // Assess node right
  assess_node(visited, map, score, height, x, y + 1, rows, cols);
  // Assess node down
  assess_node(visited, map, score, height, x + 1, y, rows, cols);
  // Assess node left
  assess_node(visited, map, score, height, x, y - 1, rows, cols);
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
  char **map = calloc(1000, sizeof(char*));
  struct trailhead *head;
  struct trailhead *tmp;
  int row = 0, col;
  while(fgets(read_buffer, 255, fp) != NULL) {
    map[row] = calloc(1, sizeof(char) * strlen(read_buffer));
    strcpy(map[row], read_buffer);

    for (col = 0; col < strlen(read_buffer); col++) {
      if (strncmp(&read_buffer[col], "0", 1) == 0) {
        struct trailhead *t = malloc(sizeof(struct trailhead));
        t->x = row;
        t->y = col;
        if (head == NULL) {
          head = t;
          tmp = t;
          continue;
        }
        tmp->next = t;
        tmp = tmp->next;
      }
    }
    row++;
  }
  col--;
  //printf("Map size: (%i,%i)\n", row, col);
  //for (int i = 0; i < row; i++) {
  //  printf("%s", map[i]);
  //}

  // Assess each trailhead
  tmp = head;
  int total_score = 0;
  while (tmp != NULL) {
    printf("Assessing trailhead: (%i,%i) -> Score: ", tmp->x, tmp->y);
    struct hashtable *visited = create_hash_table(100);
    int *score = calloc(1, sizeof(int));
    assess_node(visited, map, score, -1, tmp->x, tmp->y, row, col);
    tmp = tmp->next;
    printf("%i\n", *score);
    total_score += *score;
    free(score);
    destroy_hash_table(visited);
  }

  printf("Total score: %i\n", total_score);

  tmp = head;
  while (tmp != NULL) {
    //printf("Trailhead @ (%i,%i)\n", tmp->x, tmp->y);
    struct trailhead *tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }

  for (int i = 0; i < row; i++) {
    free(map[i]);
  }

  free(map);
  fclose(fp);
  return EXIT_SUCCESS;
}