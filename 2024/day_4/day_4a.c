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
  char **t_crossword = calloc(num_cols, sizeof(char*));
  char **dr_crossword = calloc(num_rows + num_cols - 1, sizeof(char*));
  char **dl_crossword = calloc(num_rows + num_cols - 1, sizeof(char*));
  fseek(fp, 0, SEEK_SET);
  int i = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    const char *buff_copy = strdup(read_buffer);
    // Create normal crossword array
    crossword[i] = buff_copy;
    // Create transposed and diagonal crossword array
    for (int j = 0; j < strlen(buff_copy) - 1; j++) {
      if (i == 0) {
        t_crossword[j] = calloc(1, sizeof(char) * (num_cols + 1));
        if (j == 0) {
          for (int z = 0; z < strlen(buff_copy) - 1; z++) {
            dr_crossword[z] = calloc(1, sizeof(char) * (num_cols + 1));
            dl_crossword[z] = calloc(1, sizeof(char) * (num_cols + 1));
          }
        }
      } else if (j == 0) {
        dr_crossword[num_cols + i - 1] = calloc(1, sizeof(char) * (num_cols + 1));
        dl_crossword[num_cols + i - 1] = calloc(1, sizeof(char) * (num_cols + 1));
      }
      strncat(t_crossword[j], &buff_copy[j], 1);
      strncat(dr_crossword[num_cols - j - 1 + i], &buff_copy[j], 1);
      strncat(dl_crossword[i + j], &buff_copy[j], 1);
    }
    i++;
  }

  for (int i = 0; i < num_cols; i++) {
    strcat(t_crossword[i], "\n");
  }
  for (int i = 0; i < num_rows + num_cols - 1; i++) {
    strcat(dr_crossword[i], "\n");
    strcat(dl_crossword[i], "\n");
  }

  int xmas_count = 0;
  char forward[] = "XMAS";
  char backward[] = "SAMX";
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      // Check horizontal entries
      char horizontal_slice[sizeof(forward)];
      strncpy(horizontal_slice, crossword[i] + j, strlen(forward));
      if (strcmp(forward, horizontal_slice) == 0 || strcmp(backward, horizontal_slice) == 0) {
        xmas_count++;
      }
      // Check vertical entries via transpose crossword
      char vertical_slice[sizeof(forward)];
      strncpy(vertical_slice, t_crossword[j] + i, strlen(forward));
      if (strcmp(forward, vertical_slice) == 0 || strcmp(backward, vertical_slice) == 0) {
        xmas_count++;
      }
    }
  }

  for (int i = 0; i < num_rows + num_cols - 1; i++){
    if (strlen(dr_crossword[i]) - 1 < strlen(forward)) continue;
    for (int j = 0; j < strlen(dr_crossword[i]) - strlen(forward); j++) {
      // Check right diagonal entries
      char rdiagonal_slice[sizeof(forward)];
      strncpy(rdiagonal_slice, dr_crossword[i] + j, strlen(forward));
      if (strcmp(forward, rdiagonal_slice) == 0 || strcmp(backward, rdiagonal_slice) == 0) {
        xmas_count++;
      }
      // Check right diagonal entries
      char ldiagonal_slice[sizeof(forward)];
      strncpy(ldiagonal_slice, dl_crossword[i] + j, strlen(forward));
      if (strcmp(forward, ldiagonal_slice) == 0 || strcmp(backward, ldiagonal_slice) == 0) {
        xmas_count++;
      }
    }
  }
  printf("XMAS count: %i\n", xmas_count);

  for (int i = 0; i < num_cols; i++) {
    free(t_crossword[i]);
  }
  free(t_crossword);

  for (int i = 0; i < num_rows + num_cols - 1; i++) {
    free(dr_crossword[i]);
    free(dl_crossword[i]);
  }
  free(dr_crossword);
  free(dl_crossword);

  fclose(fp);
  return EXIT_SUCCESS;
}