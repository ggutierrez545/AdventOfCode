#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  char num[6];
  int n;
  int m;
} part_number;


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  int board[140][140];
  part_number pns[2000];
  part_number* curr_pn = NULL;
  int store_idx = 0;

  int i = 0;
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;
    for (int j = 0; j < strlen(line); j++) {
      char c = line[j];
      if (c == '.') {
        board[i][j] = 0;
        if (curr_pn) {
          curr_pn = NULL;
          store_idx++;
        }
      } else if (!isdigit(c)) {
        board[i][j] = 1;
        if (curr_pn) {
          curr_pn = NULL;
          store_idx++;
        }
      } else {
        char val[2] = {c, '\0'};
        if (!curr_pn) {
          part_number pn;
          pn.n = i;
          pn.m = j;
          pns[store_idx] = pn;
          curr_pn = &pns[store_idx];
        }
        strcat(curr_pn->num, val);
        board[i][j] = 3;
      }
    }
    i++;
  }

  int64_t sum = 0;
  for (int i = 0; i < 2000; i++) {
    int surround_sum = 0;
    part_number pn = pns[i];
    if (!strcmp(pn.num, ""))
      break;
    // Check for symbols on same line as pn on either side
    int left_idx = pn.m - 1, right_idx = pn.m + strlen(pn.num);
    if (left_idx >= 0)
      surround_sum += board[pn.n][left_idx];
    if (right_idx < 140)
      surround_sum += board[pn.n][right_idx];

    // Check for symbols on top / bottom / diagonals
    int n_down = pn.n + 1, n_up = pn.n - 1;
    for (int j = 0; j <= strlen(pn.num) + 1; j++) {
      int m = pn.m + j - 1;
      if ((m >= 0) && (m < 140)) {
        if (n_up >= 0) {
          surround_sum += board[n_up][m];
        }
        if (n_down < 140) {
          surround_sum += board[n_down][m];
        }
      }
    }
    if (surround_sum > 0) {
      sum += atoi(pn.num);
    }
  }

  printf("Final sum: %lld\n", sum);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
