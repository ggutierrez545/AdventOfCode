#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"


typedef struct obstacle {
  int x;
  int y;
  struct obstacle *next;
} obstacle;

typedef struct guard {
  int x;
  int y;
  char orientation;
} guard;

int load_obstacles(char *file, int *i, int *num_columns, struct obstacle *head, struct guard *guard) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(file, "r");

  if (fp == NULL) {
    printf("Error opening file: %s", file);
    return EXIT_FAILURE;
  }

  while(fgets(read_buffer, 255, fp) != NULL) {
    *num_columns = strlen(read_buffer) - 1;
    for (int j = 0; j < strlen(read_buffer); j++) {
      if (strncmp(&read_buffer[j], "#", 1) == 0) {
        if (head == NULL) {
          head = malloc(sizeof(obstacle));
          head->x = *i;
          head->y = j;
          continue;
        }
        struct obstacle *ob = malloc(sizeof(obstacle));
        ob->x = *i;
        ob->y = j;
        struct obstacle *tmp = head;
        while (tmp != NULL) {
          if (tmp->next == NULL) {
            tmp->next = ob;
            break;
          }
          tmp = tmp->next;
        }
      } else if (strncmp(&read_buffer[j], "^", 1) == 0 ||
                 strncmp(&read_buffer[j], ">", 1) == 0 ||
                 strncmp(&read_buffer[j], "v", 1) == 0 ||
                 strncmp(&read_buffer[j], "<", 1) == 0
      ) {
        guard->x = *i;
        guard->y = j;
        guard->orientation = *strndup(&read_buffer[j], 1);
      }
    }
    (*i)++;
  }
  struct obstacle *tmp = head;
  while (tmp != NULL) {
    printf("Obstacle: (%i,%i)\n", tmp->x, tmp->y);
    tmp = tmp->next;
  }
  fclose(fp);
  return EXIT_SUCCESS;
}

bool simulate_movement(struct guard *guard, struct obstacle *head, int *num_columns) {
  struct hashtable *visited = create_hash_table(1000);
  struct item *start_position = malloc(sizeof(item));
  char key[25];
  sprintf(key, "(%i,%i)", guard->x, guard->y);
  strcpy(start_position->key, key);
  hash_table_insert(visited, start_position);
  bool on_map = true;
  int steps = 0;
  while (on_map && steps < 10000) {
    if (guard->orientation == '^') {
      // Identify closest obstacle
      struct obstacle *tmp = head;
      struct obstacle *closest = NULL;
      while (tmp != NULL) {
        bool same_col = tmp->y == guard->y;
        bool not_behind = tmp->x < guard->x;
        if (same_col && not_behind) {
          if (closest == NULL) {
            closest = tmp;
            continue;
          }
          bool is_closer = tmp->x > closest->x;
          if (is_closer) {
            closest = tmp;
          }
        }
        tmp = tmp->next;
      }
      if (closest == NULL) {
        on_map = false;
        closest = malloc(sizeof(obstacle));
        closest->x = 0;
        closest->y = guard->y;
      }
      int j;
      for (j = guard->x; j > closest->x; j--) {
        char key[25];
        sprintf(key, "^(%i,%i)", j, guard->y);
        struct item *space = hash_table_lookup(visited, key);
        if (space == NULL) {
          space = malloc(sizeof(item));
          strcpy(space->key, key);
          hash_table_insert(visited, space);
        }
        steps++;
      }
      guard->x = j + 1;
      guard->orientation = on_map ? '>' : 'x';
    } else if (guard->orientation == '>') {
      // Identify closest obstacle
      struct obstacle *tmp = head;
      struct obstacle *closest = NULL;
      while (tmp != NULL) {
        bool same_row = tmp->x == guard->x;
        bool not_behind = tmp->y > guard->y;
        if (same_row && not_behind) {
          if (closest == NULL) {
            closest = tmp;
            continue;
          }
          bool is_closer = tmp->y < closest->y;
          if (is_closer) {
            closest = tmp;
          }
        }
        tmp = tmp->next;
      }
      if (closest == NULL) {
        on_map = false;
        closest = malloc(sizeof(obstacle));
        closest->x = guard->x;
        closest->y = *num_columns;
      }
      int j;
      for (j = guard->y; j < closest->y; j++) {
        char key[25];
        sprintf(key, ">(%i,%i)", guard->x, j);
        struct item *space = hash_table_lookup(visited, key);
        if (space == NULL) {
          space = malloc(sizeof(item));
          strcpy(space->key, key);
          hash_table_insert(visited, space);
        }
        steps++;
      }
      guard->y = j - 1;
      guard->orientation = on_map ? 'v' : 'x';
    } else if (guard->orientation == 'v') {
      // Identify closest obstacle
      struct obstacle *tmp = head;
      struct obstacle *closest = NULL;
      while (tmp != NULL) {
        bool same_col = tmp->y == guard->y;
        bool not_behind = tmp->x > guard->x;
        if (same_col && not_behind) {
          if (closest == NULL) {
            closest = tmp;
            continue;
          }
          bool is_closer = tmp->x < closest->x;
          if (is_closer) {
            closest = tmp;
          }
        }
        tmp = tmp->next;
      }
      if (closest == NULL) {
        on_map = false;
        closest = malloc(sizeof(obstacle));
        closest->x = *num_columns;
        closest->y = guard->y;
      }
      int j;
      for (j = guard->x; j < closest->x; j++) {
        char key[25];
        sprintf(key, "v(%i,%i)", j, guard->y);
        struct item *space = hash_table_lookup(visited, key);
        if (space == NULL) {
          space = malloc(sizeof(item));
          strcpy(space->key, key);
          hash_table_insert(visited, space);
        }
        steps++;
      }
      guard->x = j - 1;
      guard->orientation = on_map ? '<' : 'x';
    } else if (guard->orientation == '<') {
      // Identify closest obstacle
      struct obstacle *tmp = head;
      struct obstacle *closest = NULL;
      while (tmp != NULL) {
        bool same_row = tmp->x == guard->x;
        bool not_behind = tmp->y < guard->y;
        if (same_row && not_behind) {
          if (closest == NULL) {
            closest = tmp;
            continue;
          }
          bool is_closer = tmp->y > closest->y;
          if (is_closer) {
            closest = tmp;
          }
        }
        tmp = tmp->next;
      }
      if (closest == NULL) {
        on_map = false;
        closest = malloc(sizeof(obstacle));
        closest->x = guard->x;
        closest->y = 0;
      }
      int j;
      for (j = guard->y; j > closest->y; j--) {
        char key[25];
        sprintf(key, "<(%i,%i)", guard->x, j);
        struct item *space = hash_table_lookup(visited, key);
        if (space == NULL) {
          space = malloc(sizeof(item));
          strcpy(space->key, key);
          hash_table_insert(visited, space);
        }
        steps++;
      }
      guard->y = j + 1;
      guard->orientation = on_map ? '^' : 'x';
    }
  }
  return on_map;
}


int main(int argc, char *argv[]) {

  // Initial read to get obstacle coordinates
  int i = 0;
  int num_columns;
  struct obstacle *head = malloc(sizeof(struct obstacle));
  struct guard *guard = malloc(sizeof(struct guard));
  int exit;
  exit = load_obstacles(argv[1], &i, &num_columns, head, guard);
  if (exit == EXIT_FAILURE) {
    return exit;
  }

  //bool on_map = simulate_movement(guard, head, &num_columns);
  //printf("On map: %s\n", on_map ? "true" : "false");

  //printf("Head: (%i,%i)\n", head->x, head->y);
  //struct obstacle *tmp = head;
  //while (tmp != NULL) {
  //  printf("Obstacle: (%i,%i)\n", tmp->x, tmp->y);
  //  tmp = tmp->next;
  //}
  return EXIT_SUCCESS;
  int potential_obstacles = 0;
  // Simulate guard movement
  for (int j = 0; j < num_columns; j++) {
    for (int z = 0; z < num_columns; z++) {
      struct obstacle *inject = malloc(sizeof(struct obstacle));
      inject->x = j;
      inject->y = z;
      inject->next = NULL;
      struct obstacle *tmp = head;
      while (tmp->next != NULL) {
        tmp = tmp->next;
      }
      tmp->next = inject;
      struct guard *guard_copy = malloc(sizeof(struct guard));
      guard_copy->x = guard->x;
      guard_copy->y = guard->y;
      guard_copy->orientation = guard->orientation;
      printf("Guard copy: %c(%i,%i)\n", guard_copy->orientation, guard_copy->x, guard_copy->y);
      tmp = head;
      while (tmp != NULL) {
        printf("Obstacle: (%i,%i)\n", tmp->x, tmp->y);
        tmp = tmp->next;
      }
      bool trapped = simulate_movement(guard_copy, head, &num_columns);
      tmp = head;
      while (tmp->next != NULL) {
        tmp = tmp->next;
      }
      tmp->next = NULL;
      if (trapped) {
        potential_obstacles++;
      }
    }
    return EXIT_SUCCESS;
  }

  printf("Potential obstacles: %i\n", potential_obstacles);

  return EXIT_SUCCESS;
}