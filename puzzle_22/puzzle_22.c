#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  int64_t x;
  int64_t y;
  void* next;
} GalaxyNode;

typedef struct {
  int num_galaxies;
  u_int64_t idx;
  u_int64_t offset;
  void* next;
} ColumnNode;

GalaxyNode* INIT_GALAXY_NODE = NULL;
ColumnNode* INIT_COLUMN_NODE = NULL;

int FACTOR = 1000000;

void add_galaxy_node(GalaxyNode* n) {
  GalaxyNode* curr = INIT_GALAXY_NODE;
  if (!curr) {
    INIT_GALAXY_NODE = n;
    return;
  }
  while (curr) {
    if (!curr->next) {
      curr->next = n;
      break;
    }
    curr = curr->next;
  }
}

void add_column_node(ColumnNode * n) {
  ColumnNode* curr = INIT_COLUMN_NODE;
  if (!curr) {
    INIT_COLUMN_NODE = n;
    return;
  }
  while (curr) {
    if (!curr->next) {
      curr->next = n;
      break;
    }
    curr = curr->next;
  }
}

ColumnNode* get_column_node(u_int64_t idx) {
  ColumnNode* curr = INIT_COLUMN_NODE;
  while (curr) {
    if (curr->idx == idx)
      return curr;
    curr = curr->next;
  }
  return NULL;
}

int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  int row_idx = 0;
  u_int64_t row_offset = 0;
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;

    if (!INIT_COLUMN_NODE) {
      ColumnNode* prev_column = NULL;
      for (int col_idx = 0; col_idx < strlen(line); col_idx++) {
        ColumnNode* cn = malloc(sizeof(ColumnNode));
        cn->idx = col_idx;
        cn->num_galaxies = 0;
        cn->offset = 0;
        if (prev_column)
          prev_column->next = cn;
        if (!col_idx)
          INIT_COLUMN_NODE = cn;
        prev_column = cn;
      }
    }

    ColumnNode* curr_column = INIT_COLUMN_NODE;
    int row_num_galaxies = 0;
    for (int col_idx = 0; col_idx < strlen(line); col_idx++) {
      char c = line[col_idx];
      if (c == '#') {
        row_num_galaxies++;
        GalaxyNode* g = malloc(sizeof(GalaxyNode));
        g->x = row_idx + row_offset;
        g->y = col_idx;
        add_galaxy_node(g);
        if (curr_column)
          curr_column->num_galaxies++;
      }
      if (curr_column)
        curr_column = curr_column->next;
    }
    row_idx++;

    if (!row_num_galaxies)
      row_offset += FACTOR - 1;
  }

  ColumnNode* col = INIT_COLUMN_NODE;
  u_int64_t col_offset = 0;
  while(col) {
    if (!col->num_galaxies)
      col_offset += FACTOR - 1;
    col->offset += col_offset;
    col = col->next;
  }

  GalaxyNode* curr = INIT_GALAXY_NODE;
  while (curr) {
    ColumnNode* col = get_column_node(curr->y);
    curr->y += col->offset;
    curr = curr->next;
  }

  GalaxyNode* current = INIT_GALAXY_NODE;
  int64_t shortest_len_sum = 0;
  while (current) {
    GalaxyNode* secondary = current->next;
    while (secondary) {
      shortest_len_sum += llabs(current->x - secondary->x) + llabs(current->y - secondary->y);
      secondary = secondary->next;
    }
    current = current->next;
  }

  printf("Shortest path sum: %ld\n", shortest_len_sum);
  // Collect the garbage
  GalaxyNode* g_curr = INIT_GALAXY_NODE;
  while (g_curr) {
    GalaxyNode* tmp = g_curr;
    g_curr = g_curr->next;
    free(tmp);
  }

  ColumnNode* c_curr = INIT_COLUMN_NODE;
  while (c_curr) {
    ColumnNode* tmp = c_curr;
    c_curr = c_curr->next;
    free(tmp);
  }

  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
