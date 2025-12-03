#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"

#define LIST_SIZE 1000000
#define NUM_BLINKS 25

typedef struct stone {
  uint64_t value;
  struct stone *next;
} stone;

void print_stones(struct stone *tmp) {
  while (tmp != NULL) {
    printf("%li ", tmp->value);
    tmp = tmp->next;
  }
  printf("\n");
}

void blink(struct stone *tmp) {
  while (tmp != NULL) {
    char v[100];
    sprintf(v, "%li", tmp->value);
    if (!tmp->value) {
      tmp->value = 1;
      tmp = tmp->next;
    } else if (strlen(v) % 2 == 0) {
      int half = strlen(v) / 2;
      char *left = malloc(sizeof(char) * 50);
      char *right = malloc(sizeof(char) * 50);
      strncpy(left, v, half);
      strncpy(right, v + half, half);
      struct stone *r = malloc(sizeof(struct stone));
      char *endptr;
      r->value = strtoul(right, &endptr, 10);
      tmp->value = strtoul(left, &endptr, 10);
      r->next = tmp->next;
      tmp->next = r;
      tmp = r->next;
    } else {
      tmp->value = tmp->value * 2024;
      tmp = tmp->next;
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
  struct stone *head = malloc(sizeof(struct stone));
  while(fgets(read_buffer, 255, fp) != NULL) {
    char *token = strtok(read_buffer, " ");
    char *endptr;
    head->value = strtoul(token, &endptr, 10);
    struct stone *tmp = head;
    token = strtok(NULL, " \n");
    while (token != NULL) {
      struct stone *s = malloc(sizeof(struct stone));
      char *endptr;
      s->value = strtoul(token, &endptr, 10);
      tmp->next = s;
      tmp = tmp->next;
      token = strtok(NULL, " \n");
    }
  }

  struct stone *tmp;
  //print_stones(tmp);

  for (int i = 0; i < NUM_BLINKS; i++) {
    tmp = head;
    blink(tmp);
  }

  uint64_t num_stones = 0;
  tmp = head;
  while (tmp != NULL) {
    num_stones++;
    tmp = tmp->next;
  }
  printf("Num stones: %li\n", num_stones);

  fclose(fp);
  return EXIT_SUCCESS;
}