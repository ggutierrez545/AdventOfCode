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


int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  // Initial read to get obstacle coordinates
  int i = 0;
  int num_columns;
  struct obstacle *head = NULL;
  struct guard *guard = malloc(sizeof(guard));
  while(fgets(read_buffer, 255, fp) != NULL) {
    num_columns = strlen(read_buffer) - 1;
    for (int j = 0; j < strlen(read_buffer); j++) {
      if (strncmp(&read_buffer[j], "#", 1) == 0) {
        if (head == NULL) {
          head = malloc(sizeof(obstacle));
          head->x = i;
          head->y = j;
          continue;
        }
        struct obstacle *ob = malloc(sizeof(obstacle));
        ob->x = i;
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
        guard->x = i;
        guard->y = j;
        guard->orientation = *strndup(&read_buffer[j], 1);
      }
    }
    i++;
  }

  // Simulate guard movement
  struct hashtable *visited = create_hash_table(1000);
  struct item *start_position = malloc(sizeof(item));
  char key[25];
  sprintf(key, "(%i,%i)", guard->x, guard->y);
  strcpy(start_position->key, key);
  hash_table_insert(visited, start_position);
  bool on_map = true;
  printf("Guard starting position: (%i,%i) facing %c\n", guard->x, guard->y, guard->orientation);
  while (on_map) {
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
      for (j = guard->x - 1; j > closest->x; j--) {
        char key[25];
        sprintf(key, "(%i,%i)", j, guard->y);
        printf("%s\n", key);
        struct item *space = hash_table_lookup(visited, key);
        if (space != NULL) {
          continue;
        }
        space = malloc(sizeof(item));
        strcpy(space->key, key);
        hash_table_insert(visited, space);
      }
      guard->x = j + 1;
      guard->orientation = on_map ? '>' : 'x';
      printf("New guard position: (%i,%i) facing %c\n", guard->x, guard->y, guard->orientation);
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
        closest->y = num_columns;
      }
      int j;
      for (j = guard->y + 1; j < closest->y; j++) {
        char key[25];
        sprintf(key, "(%i,%i)", guard->x, j);
        printf("%s\n", key);
        struct item *space = hash_table_lookup(visited, key);
        if (space != NULL) {
          continue;
        }
        space = malloc(sizeof(item));
        strcpy(space->key, key);
        hash_table_insert(visited, space);
      }
      guard->y = j - 1;
      guard->orientation = on_map ? 'v' : 'x';
      printf("New guard position: (%i,%i) facing %c\n", guard->x, guard->y, guard->orientation);
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
        closest->x = num_columns;
        closest->y = guard->y;
      }
      int j;
      for (j = guard->x + 1; j < closest->x; j++) {
        char key[25];
        sprintf(key, "(%i,%i)", j, guard->y);
        printf("%s\n", key);
        struct item *space = hash_table_lookup(visited, key);
        if (space != NULL) {
          continue;
        }
        space = malloc(sizeof(item));
        strcpy(space->key, key);
        hash_table_insert(visited, space);
      }
      guard->x = j - 1;
      guard->orientation = on_map ? '<' : 'x';
      printf("New guard position: (%i,%i) facing %c\n", guard->x, guard->y, guard->orientation);
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
      for (j = guard->y - 1; j > closest->y; j--) {
        char key[25];
        sprintf(key, "(%i,%i)", guard->x, j);
        printf("%s\n", key);
        struct item *space = hash_table_lookup(visited, key);
        if (space != NULL) {
          continue;
        }
        space = malloc(sizeof(item));
        strcpy(space->key, key);
        hash_table_insert(visited, space);
      }
      guard->y = j + 1;
      guard->orientation = on_map ? '^' : 'x';
      printf("New guard position: (%i,%i) facing %c\n", guard->x, guard->y, guard->orientation);
    }
  }

  printf("Guard exited at: (%i, %i)\n", guard->x, guard->y);
  //print_table(visited);
  int spots_visited = size_hash_table(visited);
  printf("Number of unique spots visited: %i\n", spots_visited);
  destroy_hash_table(visited);

  fclose(fp);
  return EXIT_SUCCESS;
}