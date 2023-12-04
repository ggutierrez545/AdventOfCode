#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  int board[140][140];
  char store[2500][6] = {""};
  char (*curr_num)[6] = NULL;
  int store_idx = 0;

  int i = 0;
  while (getline(&line, &len, fptr) != -1) {
    if (i > 1)
      break;
    line[strcspn(line, "\n")] = 0;
    printf("%s\n", line);
    for (int j = 0; j < strlen(line); j++) {
      char c = line[j];
      if (c == '.') {
        board[i][j] = 0;
        if (curr_num) {
          printf("New value: %s\n", *curr_num);
          curr_num = NULL;
          store_idx++;
        }
      } else if (!isdigit(c)) {
        board[i][j] = 1;
        if (curr_num) {
          printf("New value: %s\n", *curr_num);
          curr_num = NULL;
          store_idx++;
        }
      } else {
        printf("Number seen: %c\n", c);
        if (!curr_num)
          curr_num = &store[store_idx];
        strcat(store[store_idx], &c);
        board[i][j] = 3;
      }
    }
    i++;
  }

  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
