#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
  char* key;
  int value;
} item;

item color_dict[] = {
  {"red", 12},
  {"green", 13},
  {"blue", 14},
};

size_t hashtable_size = sizeof(color_dict) / sizeof(item);

int* max_lookup(const char* key) {
  for (size_t i = 0; i < hashtable_size; i++) {
    if (strcmp(color_dict[i].key, key) == 0) {
      return &color_dict[i].value;
    }
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

  int id_sum = 0;

  while (getline(&line, &len, fptr) != -1) {

    char* game = strtok(line, ":");
    char* results = strtok(NULL, ":");

    char* _ = strtok(game, " ");
    int id = atoi(strtok(NULL, " "));

    char* res_array[10];
    char* res = strtok(results, ";");
    int i = 0;
    while (res != NULL) {
      memmove(&res[0], &res[1], strlen(res));
      res_array[i] = res;
      res = strtok(NULL, ";");
      i++;
    }
    res_array[i] = NULL;

    char* pull_array[10][50];
    for (int i = 0; i < 10; i++) {
      if (!res_array[i])
        break;
      char* pull = strtok(res_array[i], ",");
      int j = 0;
      while (pull != NULL) {
        pull_array[i][j] = pull;
        pull = strtok(NULL, ",");
        j++;
      }
      pull_array[i][j] = NULL;
    }

    bool possible = true;
    for (int i = 0; i < 10; i++) {
      if (!res_array[i] || !possible)
        break;
      for (int j = 0; j < 50; j++) {
        if (!pull_array[i][j])
          break;
        char* val = strtok(pull_array[i][j], " ");
        int v = atoi(val);
        char* color = strtok(NULL, " ");

        color[strcspn(color, "\n")] = 0;
        int* max = max_lookup(color);
        if (v > *max) {
          printf("Not including: %d\n", id);
          possible = false;
          break;
        }
      }
    }

    if (possible)
      id_sum += id;
  }

  printf("ID Sum: %d\n", id_sum);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
