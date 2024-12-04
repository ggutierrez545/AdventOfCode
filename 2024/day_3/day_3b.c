#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"


int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[5000];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  char mul_target[] = "mul(";
  char dont_target[] = "don't()";
  char do_target[] = "do()";
  // Initial read
  long int sum_of_multiplications = 0;
  bool enabled = true;
  while(fgets(read_buffer, 5000, fp) != NULL) {
    printf("---\n");
    for (int i = 0; i < strlen(read_buffer) - 1; i++) {

      char mul_slice[sizeof(mul_target)];
      strncpy(mul_slice, read_buffer + i, strlen(mul_target));
      char dont_slice[sizeof(dont_target)];
      strncpy(dont_slice, read_buffer + i, strlen(dont_target));
      char do_slice[sizeof(do_target)];
      strncpy(do_slice, read_buffer + i, strlen(do_target));

      if (strcmp(dont_target, dont_slice) == 0) {
        enabled = false;
        continue;
      }

      if (strcmp(do_target, do_slice) == 0) {
        enabled = true;
        continue;
      }

      if (strcmp(mul_target, mul_slice) == 0) {
        int begin_idx = i + strlen(mul_target);
        char end_slice[sizeof(read_buffer) - i + 1];
        strncpy(end_slice, read_buffer + begin_idx, sizeof(read_buffer) - begin_idx);
        char *token = strtok(end_slice, ",");
        char *endptr1, *endptr2;
        long int val1 = strtol(token, &endptr1, 10);
        token = strtok(NULL, ")");
        long int val2 = strtol(token, &endptr2, 10);
        if (*endptr1 != '\0' || *endptr2 != '\0') continue;
        printf("Multiplying %s: %li & %li\n", enabled ? "enabled" : "disabled", val1, val2);
        sum_of_multiplications += (int)enabled * (val1 * val2);
        printf("New sum: %li\n", sum_of_multiplications);
      }
    }
  }
  printf("Sum of multiplications: %li\n", sum_of_multiplications);

  fclose(fp);
  return EXIT_SUCCESS;
}