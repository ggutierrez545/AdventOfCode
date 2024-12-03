#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"


bool is_increasing(int left, int right) {
  return left < right;
}

bool is_decreasing(int left, int right) {
  return left > right;
}

bool within_bounds(int left, int right) {
  return abs(left - right) <= 3;
}

bool is_safe(int left, int right) {
  return (is_increasing(left, right) || is_decreasing(left, right)) && within_bounds(left, right);
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
  int safe_reports = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    char *prev_token = strtok(read_buffer, " ");
    char *token = strtok(NULL, " \n");

    bool safe = true;
    bool increasing = is_increasing(atoi(prev_token), atoi(token));
    bool consistent;
    printf("%s ", prev_token);
    while (token != NULL && safe) {
      int prev_val = atoi(prev_token);
      int val = atoi(token);
      consistent = increasing == is_increasing(prev_val, val);
      safe &= is_safe(prev_val, val) && consistent;
      prev_token = token;
      token = strtok(NULL, " \n");
      printf("%s ", prev_token);
    }
    safe_reports += (int)safe;
    printf("\t-> %s\n", safe ? "safe" : "unsafe");
  }
  printf("Safe reports: %i\n", safe_reports);

  fclose(fp);
  return EXIT_SUCCESS;
}