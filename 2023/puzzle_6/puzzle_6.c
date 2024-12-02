#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


typedef struct {
  int n;
  int m;
} coord;

typedef struct {
  char num[6];
} part_number;

typedef struct {
  coord key;
  part_number* pn;
} pn_item;

pn_item pn_dict[10000] = {};

size_t p_hashtable_size = sizeof(pn_dict) / sizeof(pn_item);

part_number* pn_lookup(const coord* key) {
  for (size_t i = 0; i < p_hashtable_size; i++) {
    coord pn_coord = pn_dict[i].key;
    if (pn_coord.m == key->m && pn_coord.n == key->n) {
      return pn_dict[i].pn;
    }
  }
  return NULL;
}


bool in(part_number* ptr, part_number* array[8]) {
  for (int i = 0; i < 8; i ++) {
    if (ptr == array[i])
      return true;
  }
  return false;
}


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  coord gears[1000];
  part_number pns[2000];
  part_number* curr_pn = NULL;
  int store_idx = 0;
  int gear_idx = 0;
  int pn_dict_idx = 0;

  int i = 0;
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;
    for (int j = 0; j < strlen(line); j++) {
      char c = line[j];
      if (c == '*') {
        coord gear = {.n=i, .m=j};
        gears[gear_idx] = gear;
        gear_idx++;
        if (curr_pn) {
          curr_pn = NULL;
          store_idx++;
        }
      } else if (!isdigit(c) || c == '.') {
        if (curr_pn) {
          curr_pn = NULL;
          store_idx++;
        }
      } else {
        char val[2] = {c, '\0'};
        if (!curr_pn) {
          part_number pn;
          pns[store_idx] = pn;
          curr_pn = &pns[store_idx];
        }
        coord idx = {.n=i, .m=j};
        pn_item pn_entry = {.key=idx, .pn=&pns[store_idx]};
        pn_dict[pn_dict_idx] = pn_entry;
        pn_dict_idx++;
        strcat(curr_pn->num, val);
      }
    }
    i++;
  }

  int64_t sum = 0;
  for (int i = 0; i < gear_idx; i++) {
    coord gear = gears[i];
    printf("Looking at gear: (%i, %i)\n", gear.n, gear.m);

    coord adjacent[8];
    int adj_idx = 0;
    int left = gear.m - 1, right = gear.m + 1;

    if (left >= 0) {
      coord l = {.n=gear.n, .m=left};
      adjacent[adj_idx] = l;
      adj_idx++;
    }
    if (right < 140) {
      coord r = {.n = gear.n, .m = right};
      adjacent[adj_idx] = r;
      adj_idx++;
    }

    int n_up = gear.n - 1, n_down = gear.n + 1;
    for (int j = 0; j <= 2; j++) {
      int m = gear.m + j - 1;
      if ((m >= 0) && (m < 140)) {
        if (n_up >= 0) {
          coord u = {.n=n_up, .m=m};
          adjacent[adj_idx] = u;
          adj_idx++;
        }
        if (n_down < 140) {
          coord d = {.n=n_down, .m=m};
          adjacent[adj_idx] = d;
          adj_idx++;
        }
      }
    }

    int32_t ratio = 1;
    part_number* pns[8];
    int pn_idx = 0, unique_count = 0;

    for (int j = 0; j < adj_idx; j++) {
      coord adj = adjacent[j];
      part_number* ptr = pn_lookup(&adj);
      if (!ptr || in(ptr, pns))
        continue;
      printf("Using %s in ratio\n", ptr->num);
      pns[pn_idx] = ptr;
      ratio *= atoi(ptr->num);
      unique_count++;
      pn_idx++;
    }

    if (unique_count == 2) {
      printf("Added\n");
      sum += ratio;
    }
  }

  printf("Final sum: %lld\n", sum);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
