#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"


int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  // Initial read
  int num_entries = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    char *token = strtok(read_buffer, " ");
    while (token != NULL) {
      printf("%s", token);
      token = strtok(NULL, " \n");
    }
    printf("\n");
    num_entries++;
  }

  // Second read
  fseek(fp, 0, SEEK_SET);
  while(fgets(read_buffer, 255, fp) != NULL) {
    ...
  }

  fclose(fp);
  return EXIT_SUCCESS;
}