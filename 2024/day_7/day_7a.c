#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"



bool assess_node(uint64_t **values, uint64_t target, uint64_t current_value, int next_idx) {
  if (current_value == target) {
    return true;
  }
  if (current_value > target || values[next_idx] == NULL) {
    return false;
  }

  uint64_t left_value = current_value + *values[next_idx];
  uint64_t right_value = current_value * *values[next_idx];

  //printf("Assessing %li + %li\n", left_value, *values[next_idx]);
  bool res1 = assess_node(values, target, left_value, next_idx + 1);
  if (res1) return true;
  //printf("Assessing %li * %li\n", right_value, *values[next_idx]);
  bool res2 = assess_node(values, target, right_value, next_idx + 1);
  if (res2) return true;

  return false;
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
  int row = 0;
  uint64_t calibration_sum = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    char *token = strtok(read_buffer, ":");
    char *endptr;
    uint64_t target = strtoll(token, &endptr, 10);
    uint64_t **values = calloc(1000, sizeof(uint64_t*));
    int i = 0;
    token = strtok(NULL, " \n");
    while (token != NULL) {
      uint64_t *val = calloc(1, sizeof(uint64_t));
      char *endptr2;
      *val = strtoll(token, &endptr2, 10);
      values[i] = val;
      token = strtok(NULL, " \n");
      i++;
    }
    // printf("Assessing row: %i\n", row);
    bool has_solution = assess_node(values, target, *values[0], 1);
    calibration_sum += (int)has_solution * target;
    free(values);
    row++;
  }
  printf("Calibration sum: %li\n", calibration_sum);

  fclose(fp);
  return EXIT_SUCCESS;
}