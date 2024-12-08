#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sorting.h"
#include "../utils/hashtable.h"


int main(int argc, char *argv[]) {

  FILE *fp1, *fp2;
  char read_buffer[255];

  fp1 = fopen(argv[1], "r");
  fp2 = fopen(argv[2], "r");

  if (fp1 == NULL) {
    printf("Error opening file: %s", argv[1]);
    return EXIT_FAILURE;
  }

  if (fp2 == NULL) {
    printf("Error opening file: %s", argv[2]);
    return EXIT_FAILURE;
  }

  struct hashtable *rules = create_hash_table(1000);

  // Initial read
  while(fgets(read_buffer, 255, fp1) != NULL) {
    // Handle first page
    char *token = strtok(read_buffer, "|");
    struct item *it1 = hash_table_lookup(rules, token);
    if (it1 == NULL) {
      it1 = (struct item*)malloc(sizeof(item) + 50 * sizeof(item*));
      strcpy(it1->key, token);
      it1->before = NULL;
      hash_table_insert(rules, it1);
    }

    // Handle second page
    token = strtok(NULL, "\n");
    struct item *it2 = hash_table_lookup(rules, token);
    if (it2 == NULL) {
      it2 = (struct item*)malloc(sizeof(item) + 50 * sizeof(item*));
      strcpy(it2->key, token);
      it2->before = NULL;
      hash_table_insert(rules, it2);
    }

    // Add second page to first page's entry
    struct node *entry = (struct node*)malloc(sizeof(node));
    strcpy(entry->key, it2->key);
    entry->next = NULL;
    struct node *tmp = it1->before;
    while (tmp != NULL) {
      if (tmp->next == NULL) break;
      tmp = tmp->next;
    }
    if (tmp == NULL) {
      it1->before = entry;
    } else {
      tmp->next = entry;
    }
  }

  int middle_sum = 0;
  while(fgets(read_buffer, 255, fp2) != NULL) {
    char *buff_copy = strdup(read_buffer);
    struct hashtable *seen = create_hash_table(1000);
    char *token = strtok(buff_copy, ",");
    bool correctly_ordered = true;
    while (token != NULL && correctly_ordered) {
      struct item *item_seen = hash_table_lookup(seen, token);
      // If page hasn't been seen, add it to seen table and go to next page
      if (item_seen == NULL) {
        item_seen = (struct item*)malloc(sizeof(item));
        strcpy(item_seen->key, token);
        hash_table_insert(seen, item_seen);
      }
      // Look up item's rules
      struct item *item_rules = hash_table_lookup(rules, token);
      if (item_rules == NULL) continue;
      struct node *tmp = item_rules->before;
      while (tmp != NULL) {
        struct item *no_bueno = hash_table_lookup(seen, tmp->key);
        if (no_bueno != NULL) {
          correctly_ordered = false;
          break;
        }
        tmp = tmp->next;
      }
      token = strtok(NULL, ",\n");
    }
    if (correctly_ordered) {
      char *count_copy = strdup(read_buffer);
      int i;
      for (i = 0; count_copy[i]; count_copy[i]==',' ? i++ : *count_copy++);
      buff_copy = strdup(read_buffer);
      token = strtok(buff_copy, ",");
      int iters = 0;
      while (iters < (i / 2)) {
        token = strtok(NULL, ",\n");
        iters++;
      }
      middle_sum += atoi(token);
    }
  }
  printf("Middle sum: %i\n", middle_sum);

  fclose(fp1);
  fclose(fp2);
  return EXIT_SUCCESS;
}