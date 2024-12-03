#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_KEY_SIZE 256

typedef struct item {
  char key[MAX_KEY_SIZE];
  int count;
  struct item *next;
} item;

typedef struct hashtable {
  int capacity;
  struct item *entries[];
} hashtable;

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash(const char* key, int table_size) {
  uint64_t hash = FNV_OFFSET;
  for (const char* p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  hash = hash % table_size;
  return hash;
}

struct hashtable* create_hash_table(int capacity) {
  hashtable *ht = (struct hashtable*)malloc(sizeof(struct hashtable) + capacity * sizeof(struct item));
  ht->capacity = capacity;
  for (int i = 0; i < ht->capacity; i++) {
    ht->entries[i] = NULL;
  }
  return ht;
}

void print_table(struct hashtable *ht) {
  printf("--------\n");
  for (int i = 0; i < ht->capacity; i++) {
    if (ht->entries[i] == NULL) {
      printf("\t%i\t---\n", i);
    } else {
      printf("\t%i\t", i);
      item *tmp = ht->entries[i];
      while (tmp != NULL) {
        printf("%s - ", tmp->key);
        tmp = tmp->next;
      }
      printf("\n");
    }
  }
  printf("--------\n");
}

bool hash_table_insert(struct hashtable *ht, struct item *it) {
  if (it == NULL) return false;
  int index = hash(it->key, ht->capacity);
  it->next = ht->entries[index];
  ht->entries[index] = it;
  return true;
}

struct item *hash_table_lookup(struct hashtable *ht, char *key) {
  int index = hash(key, ht->capacity);
  item *tmp = ht->entries[index];
  while (tmp != NULL && strncmp(tmp->key, key, MAX_KEY_SIZE) != 0) {
    tmp = tmp->next;
  }
  return tmp;
}

struct item *hash_table_delete(struct hashtable *ht, char *key) {
  int index = hash(key, ht->capacity);
  item *tmp = ht->entries[index];
  item *prev = NULL;
  while (tmp != NULL && strncmp(tmp->key, key, MAX_KEY_SIZE) != 0) {
    prev = tmp;
    tmp = tmp->next;
  }
  if (tmp == NULL) return NULL;
  if (prev == NULL) {
    ht->entries[index] = tmp->next;
  } else {
    prev->next = tmp->next;
  }
  return tmp;
}

void destroy_hash_table(struct hashtable *ht) {
  for (int i = 0; i < ht->capacity; i++) {
    struct item *it = ht->entries[i];
    while (it != NULL) {
      struct item *tmp = it;
      it = it->next;
      free(tmp);
    }
  }
  free(ht);
}

// int main(void) {
//
//   int capacity = 2;
//   struct hashtable *ht = create_hash_table(capacity);
//
//   item one = {.key="1", .count=1};
//   item two = {.key="2", .count=2};
//   item three = {.key="3", .count=3};
//
//   hash_table_insert(ht, &one);
//   hash_table_insert(ht, &two);
//   hash_table_insert(ht, &three);
//
//   print_table(ht);
//
//   item *tmp = hash_table_lookup(ht, "2");
//   printf("Found %s with value %i\n", tmp->key, tmp->count);
//
//   free(ht);
//   return EXIT_SUCCESS;
// }