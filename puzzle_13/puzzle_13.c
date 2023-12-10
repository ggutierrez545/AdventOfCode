#define PCRE2_CODE_UNIT_WIDTH 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre2.h>


char** apply_regex(PCRE2_SPTR* p, PCRE2_SPTR* s) {

  char** out = malloc(100000);

  int out_idx = 0;
  PCRE2_SPTR pattern = *p, subject = *s;

  pcre2_code* re;
  int crlf_is_newline;
  int errornumber;
  int rc;
  int utf8;
  uint32_t option_bits, newline;
  PCRE2_SIZE erroroffset;
  PCRE2_SIZE* ovector;

  size_t subject_len;
  pcre2_match_data* match_data;

  re = pcre2_compile(
  pattern,
  PCRE2_ZERO_TERMINATED,
  0,
  &errornumber,
  &erroroffset,
  NULL
  );

  match_data = pcre2_match_data_create_from_pattern(re, NULL);

  subject_len = strlen((char*)subject);

  rc = pcre2_match(
  re,
  subject,
  subject_len,
  0,
  0,
  match_data,
  NULL
  );

  ovector = pcre2_get_ovector_pointer(match_data);

  (void)pcre2_pattern_info(re, PCRE2_INFO_ALLOPTIONS, &option_bits);
  utf8 = (option_bits & PCRE2_UTF) != 0;

  (void)pcre2_pattern_info(re, PCRE2_INFO_NEWLINE, &newline);
  crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
                    newline == PCRE2_NEWLINE_CRLF ||
                    newline == PCRE2_NEWLINE_ANYCRLF;

  for (int i = 0; i < rc; i++) {
    char* digit = malloc(200);
    digit[0] = '\0';
    PCRE2_SPTR substring_start = subject + ovector[2*i];
    size_t substring_length = ovector[2*i+1] - ovector[2*i];
    strncat(digit, (char*)substring_start, substring_length);
    out[out_idx] = digit;
    out_idx++;
  }

  for (;;) {
    uint32_t options = 0;
    PCRE2_SIZE start_offset = ovector[1];

    if (ovector[0] == ovector[1])
    {
      if (ovector[0] == subject_len) break;
      options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
    }

    rc = pcre2_match(
    re,
    subject,
    subject_len,
    start_offset,
    options,
    match_data,
    NULL
    );

    if (rc == PCRE2_ERROR_NOMATCH) {
      if (options == 0) break;
      ovector[1] = start_offset + 1;
      if (crlf_is_newline &&
          start_offset < subject_len - 1 &&
          subject[start_offset] == '\r' &&
          subject[start_offset + 1] == '\n'
      )
        ovector[1] += 1;
      else if (utf8)
      {
        while (ovector[1] < subject_len) {
          if ((subject[ovector[1]] & 0xc0) != 0x80) break;
          ovector[1] += 1;
        }
      }
      continue;
    }

    for (int i = 0; i < rc; i++) {
      char* digit = malloc(200);
      digit[0] = '\0';
      PCRE2_SPTR substring_start = subject + ovector[2*i];
      size_t substring_length = ovector[2*i+1] - ovector[2*i];
      strncat(digit, (char*)substring_start, substring_length);
      out[out_idx] = digit;
      out_idx++;
    }
  }
  return out;
}


void swap(uint64_t* xp, uint64_t* yp)
{
  uint64_t temp = *xp;
  *xp = *yp;
  *yp = temp;
}

// Function to perform Selection Sort
void selectionSort(uint64_t arr[], int n)
{
  int i, j, min_idx;

  // One by one move boundary of
  // unsorted subarray
  for (i = 0; i < n - 1; i++) {
    // Find the minimum element in
    // unsorted array
    min_idx = i;
    for (j = i + 1; j < n; j++)
      if (arr[j] < arr[min_idx])
        min_idx = j;

    // Swap the found maximum element
    // with the first element
    swap(&arr[min_idx], &arr[i]);
  }
}

typedef struct {
  char key;
  int index;
} item;

typedef struct {
  uint64_t hand;
  int bid;
} entry;

item card_idxs[] = {
  {'A', 0},
  {'K', 1},
  {'Q', 2},
  {'J', 3},
  {'T', 4},
  {'9', 5},
  {'8', 6},
  {'7', 7},
  {'6', 8},
  {'5', 9},
  {'4', 10},
  {'3', 11},
  {'2', 12}
};

int card_points[13] = {
  14,
  13,
  12,
  11,
  10,
  9,
  8,
  7,
  6,
  5,
  4,
  3,
  2
};

size_t hashtable_size = sizeof(card_idxs) / sizeof(item);

int* idx_lookup(const char* key) {
  for (size_t i = 0; i < hashtable_size; i++) {
    if (card_idxs[i].key == *key) {
      return &card_idxs[i].index;
    }
  }
  return NULL;
}

entry entry_dict[1000];

int* bid_lookup(const uint64_t* hand) {
  for (size_t i = 0; i < 1000; i++) {
    if (entry_dict[i].hand == *hand) {
      return &entry_dict[i].bid;
    }
  }
  return NULL;
}


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  PCRE2_SPTR pattern, subject;

  int hand_idx = 0;
  uint64_t hands[1000];
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;

    // Idxs for A, K, Q, J, T, 9, ..., 2
    int counts[13] = {0};

    subject = (PCRE2_SPTR) line;
    pattern = (PCRE2_SPTR) "(?!\\s)[AKQJT0-9]\\w*";
    char **results = apply_regex(&pattern, &subject);

    int strength[6] = {0};

    for (int i = 0; i < 5; i++) {
      int* idx = idx_lookup(&results[0][i]);
      strength[i+1] = card_points[*idx];
      counts[*idx]++;
    }

    for (int i = 0; i < 13; i++){
      switch (counts[i]) {
        case 2:
          strength[0] += 1;
          break;
        case 3:
          strength[0] += 3;
          break;
        case 4:
          strength[0] = 5;
          break;
        case 5:
          strength[0] = 6;
          break;
      }
    }

    char value_str[12];
    snprintf(
      value_str,
      12,
      "%d%02d%02d%02d%02d%02d",
      strength[0],
      strength[1],
      strength[2],
      strength[3],
      strength[4],
      strength[5]
      );

    char* ptr;
    uint64_t value = strtoul(value_str, &ptr, 10);
    hands[hand_idx] = value;

    entry e = {.hand = value, .bid = atoi(results[1])};
    entry_dict[hand_idx] = e;
    hand_idx++;
  }

  selectionSort(hands, 1000);

  uint64_t winnings = 0;
  for (int i = 0; i < 1000; i++) {
    winnings += *bid_lookup(&hands[i]) * (i + 1);
  }

  printf("Total winnings: %lu\n", winnings);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
