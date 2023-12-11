#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  char type;
  int x;
  int y;
  int boundary_piece;
} pipe;

typedef struct {
  char type;
  char prev_direction;
  int dir_idx;
} next_dir;

pipe grid[140][140];
pipe* start;

char compatible_pipes[4][4] = {
  "F7|", // up
  "-FL", // left
  "|LJ", // down
  "-J7" // right
};

next_dir next_dirs[12] = {
  {'F', 'u', 3},
  {'F', 'l', 2},
  {'7', 'r', 2},
  {'7', 'u', 1},
  {'L', 'd', 3},
  {'L', 'l', 0},
  {'J', 'r', 0},
  {'J', 'd', 1},
  {'|', 'u', 0},
  {'|', 'd', 2},
  {'-', 'r', 3},
  {'-', 'l', 1},
};

int* find_next_dir_idx(char pipe, char prev_dir) {
  for (int i = 0; i < 12; i++) {
    if (next_dirs[i].type == pipe && next_dirs[i].prev_direction == prev_dir)
      return &next_dirs[i].dir_idx;
  }
  return NULL;
}

char directions[4] = {
  'u',
  'l',
  'd',
  'r'
};

int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  int row_idx = 0;
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;

    for (int col_idx = 0; col_idx < 140; col_idx++) {
      char pipe_type = line[col_idx];
      grid[row_idx][col_idx] = (pipe){.type=pipe_type, .x=row_idx, .y=col_idx, .boundary_piece=0};
      if (pipe_type == 'S') {
        grid[row_idx][col_idx].boundary_piece = 1;
        start = &grid[row_idx][col_idx];
      }
    }
  row_idx++;
  }

  pipe* curr = start;
  int steps = 0;
  char last_move = '.';
  pipe* boundary[20000];
  while (curr != start || steps == 0) {

    int curr_x = curr->x, curr_y = curr->y;
    int adj_coords[4][2] = {
      {curr_x - 1, curr_y}, // up
      {curr_x, curr_y - 1}, // left
      {curr_x + 1, curr_y}, // down
      {curr_x, curr_y + 1} // right
    };

    // Need to pick an initial direction to move from S
    if (last_move == '.') {
      int found_adj = 0;
      for (int i = 0; i < 4; i++) {
        if (found_adj)
          break;
        pipe* adj_pipe = &grid[adj_coords[i][0]][adj_coords[i][1]];
        for (int j = 0; j < 4; j++) {
          if (adj_pipe->type == compatible_pipes[i][j]) {
            curr = adj_pipe;
            last_move = directions[i];
            found_adj = 1;
            break;
          }
        }
      }
      // Follow the pipes
    } else {
      int* next_dir_idx = find_next_dir_idx(curr->type, last_move);
      curr = &grid[adj_coords[*next_dir_idx][0]][adj_coords[*next_dir_idx][1]];
      curr->boundary_piece = 1;
      last_move = directions[*next_dir_idx];
    }
    boundary[steps] = curr;
    steps++;
  }

  // Implement ray casting algorithm
  int num_contained = 0;
  for (int i = 1; i < 139; i++) {
    for (int j = 1; j < 139; j++) {
      pipe* tile = &grid[i][j];
      if (tile->boundary_piece)
        continue;
      int num_tiles_right = 140 - tile->y;
      int traversals = 0;
      for (int z = 1; z < num_tiles_right; z++) {
        pipe* node = &grid[tile->x][tile->y + z];
        if (node->boundary_piece) {
          if (
            node->type == '|' ||
            node->type == 'L' ||
            node->type == 'J')
            traversals++;
        }
      }
      if (traversals % 2)
          num_contained++;
    }
  }

  printf("Contained tiles: %d\n", num_contained);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
