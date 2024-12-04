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

  char target[] = "mul(";
  // Initial read
  long int sum_of_multiplications = 0;
  while(fgets(read_buffer, 5000, fp) != NULL) {
    for (int i = 0; i < strlen(read_buffer) - 1; i++) {
      char slice[sizeof(target)];
      strncpy(slice, read_buffer + i, strlen(target));
      if (strcmp(target, slice) == 0) {
        int begin_idx = i + strlen(target);
        char end_slice[sizeof(read_buffer) - i + 1];
        strncpy(end_slice, read_buffer + begin_idx, sizeof(read_buffer) - begin_idx);
        char *token = strtok(end_slice, ",");
        char *endptr1, *endptr2;
        long int val1 = strtol(token, &endptr1, 10);
        token = strtok(NULL, ")");
        long int val2 = strtol(token, &endptr2, 10);
        if (*endptr1 != '\0' || *endptr2 != '\0') continue;
        sum_of_multiplications += val1 * val2;
      }
    }
  }
  printf("Sum of multiplications: %li\n", sum_of_multiplications);

  fclose(fp);
  return EXIT_SUCCESS;
}