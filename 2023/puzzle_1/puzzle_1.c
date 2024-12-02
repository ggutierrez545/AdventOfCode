#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void) {
  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("puzzle1_input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  long value = 0;
  while (getline(&line, &len, fptr) != -1) {
    int str_len = strlen(line);
    int number = 10;
    for (int i = 0; i < str_len; i++) {
      if (isdigit(line[i])) {
        number *= (line[i] - '0');
        break;
      }
    }
    for (int i = str_len; i >= 0; i--) {
      if (isdigit(line[i])) {
        number += (line[i] - '0');
        break;
      }
    }
    value += number;

  }

  printf("Final sum: %ld\n", value);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
