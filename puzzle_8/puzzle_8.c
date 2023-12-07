#define PCRE2_CODE_UNIT_WIDTH 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre2.h>
#include <math.h>


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


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  PCRE2_SPTR pattern, subject;

  int64_t card_counter[200] = {0};
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;

    subject = (PCRE2_SPTR)line;
    pattern = (PCRE2_SPTR)"\\d+(?=:)";
    char** card_id = apply_regex(&pattern, &subject);
    int id = atoi(card_id[0]) - 1;

    pattern = (PCRE2_SPTR)"(?<=:).*(?=\\|)";
    char** all_win_numbers = apply_regex(&pattern, &subject);

    pattern = (PCRE2_SPTR)"(?<=\\|).*";
    char** all_my_numbers = apply_regex(&pattern, &subject);

    pattern = (PCRE2_SPTR)"\\d+";
    subject = (PCRE2_SPTR)all_win_numbers[0];
    char** win_numbers = apply_regex(&pattern, &subject);

    subject = (PCRE2_SPTR)all_my_numbers[0];
    char** my_numbers = apply_regex(&pattern, &subject);

    int matches = 0;
    for (int i = 0; i < 200; i++) {
      if (!my_numbers[i])
        break;
      for (int j = 0; j < 200; j++) {
        if (!win_numbers[j])
          break;
        if (!strcmp(my_numbers[i], win_numbers[j])) {
          matches++;
        }
      }
    }

    // Count our original copy of this card
    card_counter[id]++;
    // For however many additional copies of this card we have
    if (matches) {
      for (int j = 1; j <= matches; j++)
        card_counter[id + j] += card_counter[id];
    }
  }

  int total = 0;
  for (int i = 0; i < 200; i ++) {
    printf("Card %d total: %ld\n", i+1, card_counter[i]);
    total += card_counter[i];
  }

  printf("Total score: %d\n", total);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
