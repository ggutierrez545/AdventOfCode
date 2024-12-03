#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/hashtable.h"


int main(int argc, char *argv[]) {

  FILE *fp;
  char read_buffer[255];

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  int num_entries = 0;
  while(fgets(read_buffer, 255, fp) != NULL) {
    num_entries++;
  }

  struct hashtable *ht1 = create_hash_table(num_entries * 2);
  struct hashtable *ht2 = create_hash_table(num_entries * 2);

  fseek(fp, 0, SEEK_SET);
  while(fgets(read_buffer, 255, fp) != NULL) {
    char *token = strtok(read_buffer, " ");
    struct item *it1 = hash_table_lookup(ht1, token);
    if (it1 == NULL) {
      it1 = (struct item*)malloc(sizeof(item));
      strcpy(it1->key, token);
      it1->count = 1;
      hash_table_insert(ht1, it1);
    } else {
      (it1->count)++;
    }
    token = strtok(NULL, " \n");
    struct item *it2 = hash_table_lookup(ht2, token);
    if (it2 == NULL) {
      it2 = (struct item*)malloc(sizeof(item));
      strcpy(it2->key, token);
      it2->count = 1;
      hash_table_insert(ht2, it2);
    } else {
      (it2->count)++;
    }
  }

  int similarity_score = 0;
  for (int i = 0; i < ht1->capacity; i++) {
    struct item *it1 = ht1->entries[i];
    if (it1 == NULL) continue;
    while (it1 != NULL) {
      struct item *it2 = hash_table_lookup(ht2, it1->key);
      if (it2 != NULL) {
        similarity_score += it1->count * (atoi(it1->key) * it2->count);
      }
      it1 = it1->next;
    }
  }
  printf("Similarity score: %i\n", similarity_score);

  destroy_hash_table(ht1);
  destroy_hash_table(ht2);

  fclose(fp);
  return EXIT_SUCCESS;
}