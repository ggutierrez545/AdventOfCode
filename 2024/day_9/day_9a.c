#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"

#define EXPANDED_SIZE 1000000

int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  // Initial read
  int file_id = 0, idx = 0;
  long expanded[EXPANDED_SIZE];
  long count[EXPANDED_SIZE];
  for (int i = 0; i < EXPANDED_SIZE; i++) {
    expanded[i] = -1;
  }
  while(fgets(read_buffer, 255, fp) != NULL) {
    for (int i = 1; i < strlen(read_buffer); i += 2) {
      //printf("File ID: %i\n", file_id);
      if (read_buffer[i - 1] == '\n') continue;
      char num_files[2];
      strncpy(num_files, &read_buffer[i-1], sizeof(char));
      char *endptr;
      int n_files = strtol(num_files, &endptr, 10);
      count[file_id] = n_files;
      //printf("Num files: %c - %s\n", num_files);
      for (int j = 0; j < n_files; j++) {
        expanded[idx] = file_id;
        idx++;
      }
      if (read_buffer[i] == '\n') continue;
      char num_free[2];
      strncpy(num_free, &read_buffer[i], sizeof(char));
      char *endptr2;
      int num_free_spaces = (int)strtol(num_free, &endptr2, 10);
      //printf("Num free spaces: %c - %c\n", read_buffer[i+1], num_free);
      idx += num_free_spaces;
      file_id++;
    }
  }
  //printf("Actual len: %li\n", idx);

  printf("Final index: %li\n", idx);
  printf("Expanded\n");
  for (int i = 0; i < idx; i++) {
    printf("%i", expanded[i]);
    if (expanded[i] == -1) {
      printf("\n");
    } else {
      printf(" - count: %i\n", count[expanded[i]]);
    }
  }

  long j_idx = idx;
  int i;
  for (i = 0; i < j_idx; i++) {
    if (expanded[i] != -1) continue;
    //if (i == 50127) {
    //  for (int o = 0; o < idx; o++) {
    //    printf("%i", expanded[o]);
    //    if (expanded[o] == -1) {
    //      printf("\n");
    //    } else {
    //      printf(" - count: %i\n", count[expanded[o]]);
    //    }
    //  }
    //  break;
    //}
    while (expanded[j_idx] == -1) {
      //if (i == 50127) {
      //  printf("%i\n", j_idx);
      //}
      j_idx--;
    }
    expanded[i] = expanded[j_idx];
    expanded[j_idx] = -1;
    j_idx--;
    //if (i == 50127) {
    //  printf("%i\n", j_idx);
    //}
  }
  //printf("Final indices: %i - %i\n", i, j_idx);

  //printf("Compressed\n");
  //for (i = 0; i < idx; i++) {
  //  printf("%i\n", expanded[i]);
  //}

  uint64_t checksum = 0;
  for (i = 0; i < EXPANDED_SIZE; i++) {
    if (expanded[i] == -1) continue;
    count[expanded[i]]--;
    //printf("Add: (%i * %i) + %li = ", i, expanded[i], checksum);
    checksum += i * expanded[i];
    //printf("%li\n", checksum);
  }

  //for (int i = 0; i <= file_id; i++) {
  //  printf("Remaining count of %i -> %i\n", i, count[i]);
  //}
  printf("Checksum: %li\n", checksum);


  fclose(fp);
  return EXIT_SUCCESS;
}