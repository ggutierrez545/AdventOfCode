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
    char* digit = malloc(400);
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
      char* digit = malloc(400);
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

typedef struct {
  char left[4];
  char right[4];
} fork;

typedef struct {
  char node[4];
  fork options;
} instruction;


instruction map[758];

fork* map_lookup(const char node[4]) {
  for (int i = 0; i < 758; i++) {
    if (!strcmp(map[i].node, node))
      return &map[i].options;
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

  char steps[278] = "";

  int line_idx = 0;
  int map_idx = 0;
  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;

    subject = (PCRE2_SPTR) line;
    pattern = (PCRE2_SPTR) "[A-Z]+";
    char **results = apply_regex(&pattern, &subject);

    if (!line_idx) {
      for (int i = 0; i < strlen(results[0]); i++)
        steps[i] = results[0][i];
      line_idx++;
      continue;
    }

    if (!results[0])
      continue;

    fork f;
    strcpy(f.left, results[1]);
    strcpy(f.right, results[2]);
    instruction inst;
    strcpy(inst.node, results[0]);
    inst.options = f;
    map[map_idx] = inst;
    map_idx++;
  }

  char current_node[4] = "AAA";
  int step_idx = 0;
  int num_steps = 0;

  while (strcmp(current_node, "ZZZ")) {
    fork* f = map_lookup(current_node);
    if (steps[step_idx] == 'L') {
      strncpy(current_node, f->left, 4);
    } else {
      strncpy(current_node, f->right, 4);
    }
    step_idx++;
    if (step_idx == strlen(steps))
      step_idx = 0;
    num_steps++;
  }

  printf("Num steps: %d\n", num_steps);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
