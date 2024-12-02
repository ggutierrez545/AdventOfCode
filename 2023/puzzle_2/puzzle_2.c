#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static const char spells[10][6] = {
  "zero",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

typedef struct {
  char* key;
  int value;
} item;

item spells_dict[] = {
  {"zero", 0},
  {"one", 1},
  {"two", 2},
  {"three", 3},
  {"four", 4},
  {"five", 5},
  {"six", 6},
  {"seven", 7},
  {"eight", 8},
  {"nine", 9}
};

size_t hashtable_size = sizeof(spells_dict) / sizeof(item);

int* digit_lookup(const char* key) {
  for (size_t i = 0; i < hashtable_size; i++) {
    if (strcmp(spells_dict[i].key, key) == 0) {
      return &spells_dict[i].value;
    }
  }
  return NULL;
}

void strpre(char* dest, char* src) {
  strcat(dest, " ");
  char to_insert = dest[0];
  dest[0] = *src;
  for (int i = 1; i < strlen(dest); i++) {
    char tmp = dest[i];
    dest[i] = to_insert;
    to_insert = tmp;
  }
}

int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("puzzle2_input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  long value = 0;
  while (getline(&line, &len, fptr) != -1) {
    int str_len = strlen(line);
    char first_slice[] = "";
    int number = 10;

    bool spell_found = false;
    for (int i = 0; i < str_len; i++) {
      if (spell_found)
        break;
      char v = line[i];
      strcat(first_slice, &v);
      if (isdigit(v)) {
        number *= (v - '0');
        break;
      } else {
        for (int j = 0; j < 10; j++) {
          char* c = strstr(first_slice, spells[j]);
          if (c) {
            number *= *digit_lookup(spells[j]);
            spell_found = true;
            break;
          }
        }
      }
    }

    char second_slice[] = "";
    spell_found = false;
    for (int i = str_len; i >= 0; i--) {
      if (spell_found)
        break;
      char v = line[i];
      if ((v == '\n') || (v == '\0'))
        continue;
      strpre(second_slice, &v);
      if (isdigit(v)) {
        number += (v - '0');
        break;
      } else {
        for (int j = 0; j < 10; j++) {
          char* c = strstr(second_slice, spells[j]);
          if (c) {
            number += *digit_lookup(spells[j]);
            spell_found = true;
            break;
          }
        }
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
