#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"


void print_crossword(const char **crossword, int num_rows) {
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < strlen(crossword[i]); j++) {
      printf("%c", crossword[i][j]);
    }
  }
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
  int num_rows = 0, num_cols = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    num_cols = strlen(read_buffer) - 1;
    num_rows++;
  }

  const char *crossword[num_rows];
  fseek(fp, 0, SEEK_SET);
  int i = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    const char *buff_copy = strdup(read_buffer);
    // Create normal crossword array
    crossword[i] = buff_copy;
    i++;
  }

  int xmas_count = 0;
  char forward[] = "MAS";
  char backward[] = "SAM";
  // 3x3 sliding window technique
  for (int i = 0; i < num_rows - 2; i++) {
    for (int j = 0; j < num_cols - 2; j++) {
      char *rdiag = malloc(sizeof(char) * 3);
      char *ldiag = malloc(sizeof(char) * 3);
      // Construct right diagonal
      strncpy(rdiag, crossword[i] + j, 1);
      strncat(rdiag, crossword[i + 1] + j + 1, 1);
      strncat(rdiag, crossword[i + 2] + j + 2, 1);
      // Construct left diagonal
      strncpy(ldiag, crossword[i] + j + 2, 1);
      strncat(ldiag, crossword[i + 1] + j + 1, 1);
      strncat(ldiag, crossword[i + 2] + j, 1);
      if (
      (strcmp(forward, rdiag) == 0 || strcmp(backward, rdiag) == 0) &&
      (strcmp(forward, ldiag) == 0 || strcmp(backward, ldiag) == 0)) {
        xmas_count++;
      }
    }
  }

  printf("XMAS count: %i\n", xmas_count);

  fclose(fp);
  return EXIT_SUCCESS;
}