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

typedef struct report {
  bool safe;
  int num_entries;
} report;

struct report *report_is_safe(char *read_buffer, int skip_idx) {
  char *prev_token = strtok(read_buffer, " ");
  char *token = strtok(NULL, " \n");

  struct report *out = (struct report*)malloc(sizeof(struct report));

  int idx = 0;
  if (skip_idx == 0) {
    prev_token = token;
    token = strtok(NULL, " \n");
    idx++;
  } else if (skip_idx == 1) {
    token = strtok(NULL, " \n");
  }

  bool safe = true;
  bool increasing = is_increasing(atoi(prev_token), atoi(token));
  bool consistent;
  printf("%s ", prev_token);
  while (token != NULL) {

    if (idx == skip_idx - 1 && skip_idx != 1) {
      token = strtok(NULL, " \n");
      if (token == NULL) break;
    }

    int prev_val = atoi(prev_token);
    int val = atoi(token);
    consistent = increasing == is_increasing(prev_val, val);
    safe &= is_safe(prev_val, val) && consistent;
    prev_token = token;
    token = strtok(NULL, " \n");
    printf("%s ", prev_token);
    idx++;
  }

  out->safe = safe;
  out->num_entries = idx + 1;
  return out;
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
    printf("---\n");
    char *buff_copy = strdup(read_buffer);
    struct report *out = report_is_safe(buff_copy, -1);
    safe_reports += (int)(out->safe);
    printf("\t-> %s\n", out->safe ? "safe" : "unsafe");
    printf("Num entries: %i\n", out->num_entries);
    if (!(out->safe)) {
      printf("Retrying unsafe reports...\n");
      struct report *retry;
      for (int i = 0; i < out->num_entries; i++) {
        buff_copy = strdup(read_buffer);
        retry = report_is_safe(buff_copy, i);
        printf("\t-> %s\n", retry->safe ? "safe" : "unsafe");
        if (retry->safe) {
          safe_reports++;
          free(retry);
          break;
        }
        free(retry);
      }
    }
    printf("Num safe: %i\n", safe_reports);
    free(out);
  }
  printf("Safe reports: %i\n", safe_reports);

  fclose(fp);
  return EXIT_SUCCESS;
}