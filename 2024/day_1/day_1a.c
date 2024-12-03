#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"


int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  int *id_list_1, *id_list_2;
  int num_entries = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    num_entries++;
  }
  printf("Num entries: %i\n", num_entries);

  id_list_1 = (int*)malloc(num_entries * sizeof(int));
  id_list_2 = (int*)malloc(num_entries * sizeof(int));

  if (id_list_1 == NULL || id_list_2 == NULL) {
    printf("Memory allocation failed.\n");
    return EXIT_FAILURE;
  }

  fseek(fp, 0, SEEK_SET);
  int i = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    char *token = strtok(read_buffer, " ");
    id_list_1[i] = atoi(token);
    token = strtok(NULL, " \n");
    id_list_2[i] = atoi(token);
    i++;
  }

  bubbleSort(id_list_1, num_entries);
  bubbleSort(id_list_2, num_entries);
  int sum_of_differences = 0;
  for (int i = 0; i < num_entries; i++) {
    printf("Elements (%i, %i)\n", id_list_1[i], id_list_2[i]);
    int abs_difference = abs(id_list_1[i] - id_list_2[i]);
    printf("Difference: %i\n", abs_difference);
    sum_of_differences = sum_of_differences + abs_difference;
  }
  printf("Sum of differences: %i\n", sum_of_differences);

  fclose(fp);
  return EXIT_SUCCESS;
}