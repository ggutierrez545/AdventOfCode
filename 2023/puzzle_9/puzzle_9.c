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

typedef struct {
  uint64_t begin_id;
  uint64_t end_id;
  uint64_t map;
} id_bin;


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("input.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  PCRE2_SPTR pattern, subject;

  int section_id = 0;
  uint64_t seed_ids[25];
  int num_seeds = 0;

  id_bin seed_to_soil[50];
  int seed_to_soil_idx = 0;

  id_bin soil_to_fertilizer[50];
  int soil_to_fertilizer_idx = 0;

  id_bin fertilizer_to_water[50];
  int fertilizer_to_water_idx = 0;

  id_bin water_to_light[50];
  int water_to_light_idx = 0;

  id_bin light_to_temp[50];
  int light_to_temp_idx = 0;

  id_bin temp_to_humid[50];
  int temp_to_humid_idx = 0;

  id_bin humid_to_loc[50];
  int humid_to_loc_idx = 0;

  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;

    // skip empty lines
    if (!strcmp(line, "")) {
      section_id++;
      continue;
    }

    subject = (PCRE2_SPTR) line;
    pattern = (PCRE2_SPTR) "\\d+";
    char **results = apply_regex(&pattern, &subject);

    // skip map title lines
    if (!results[0])
      continue;

    id_bin bin;
    char* ptr;
    bin.map = strtoull(results[0], &ptr, 10);
    bin.begin_id = strtoull(results[1], &ptr, 10);
    bin.end_id = bin.begin_id + strtoull(results[2], &ptr, 10);

    switch (section_id) {
      // Base seed ids case
      case 0:
        for (int i = 0; i < 25; i++) {
          if (!results[i])
            break;
          seed_ids[i] = strtoull(results[i], &ptr, 10);
          num_seeds++;
        }
        break;
      // seed-to-soil map case
      case 1:
        seed_to_soil[seed_to_soil_idx] = bin;
        seed_to_soil_idx++;
        break;
      // soil-to-fertilizer map case
      case 2:
        soil_to_fertilizer[soil_to_fertilizer_idx] = bin;
        soil_to_fertilizer_idx++;
        break;
      // fertilizer-to-water map case
      case 3:
        fertilizer_to_water[fertilizer_to_water_idx] = bin;
        fertilizer_to_water_idx++;
        break;
      // water-to-light map case
      case 4:
        water_to_light[water_to_light_idx] = bin;
        water_to_light_idx++;
        break;
      // light-to-temp map case
      case 5:
        light_to_temp[light_to_temp_idx] = bin;
        light_to_temp_idx++;
        break;
      // temp-to-humid map case
      case 6:
        temp_to_humid[temp_to_humid_idx] = bin;
        temp_to_humid_idx++;
        break;
      // humid-to-location map case
      case 7:
        humid_to_loc[humid_to_loc_idx] = bin;
        humid_to_loc_idx++;
        break;
    }
  }

  int64_t smallest_loc_id = INFINITY;
  for (int i = 0; i < num_seeds; i++) {
    uint64_t seed_id = seed_ids[i];
    uint64_t soil_id = seed_id;
    for (int j = 0; j < seed_to_soil_idx; j++) {
      id_bin bin = seed_to_soil[j];
      if ((bin.begin_id <= seed_id) && (seed_id < bin.end_id)) {
        soil_id = bin.map + seed_id - bin.begin_id;
        break;
      }
    }
    uint64_t fert_id = soil_id;
    for (int j = 0; j < soil_to_fertilizer_idx; j++) {
      id_bin bin = soil_to_fertilizer[j];
      if ((bin.begin_id <= soil_id) && (soil_id < bin.end_id)) {
        fert_id = bin.map + soil_id - bin.begin_id;
        break;
      }
    }
    uint64_t water_id = fert_id;
    for (int j = 0; j < fertilizer_to_water_idx; j++) {
      id_bin bin = fertilizer_to_water[j];
      if ((bin.begin_id <= fert_id) && (fert_id < bin.end_id)) {
        water_id = bin.map + fert_id - bin.begin_id;
        break;
      }
    }
    uint64_t light_id = water_id;
    for (int j = 0; j < water_to_light_idx; j++) {
      id_bin bin = water_to_light[j];
      if ((bin.begin_id <= water_id) && (water_id < bin.end_id)) {
        light_id = bin.map + water_id - bin.begin_id;
        break;
      }
    }
    uint64_t temp_id = light_id;
    for (int j = 0; j < light_to_temp_idx; j++) {
      id_bin bin = light_to_temp[j];
      if ((bin.begin_id <= light_id) && (light_id < bin.end_id)) {
        temp_id = bin.map + light_id - bin.begin_id;
        break;
      }
    }
    uint64_t humid_id = temp_id;
    for (int j = 0; j < temp_to_humid_idx; j++) {
      id_bin bin = temp_to_humid[j];
      if ((bin.begin_id <= temp_id) && (temp_id < bin.end_id)) {
        humid_id = bin.map + temp_id - bin.begin_id;
        break;
      }
    }
    uint64_t loc_id = humid_id;
    for (int j = 0; j < humid_to_loc_idx; j++) {
      id_bin bin = humid_to_loc[j];
      if ((bin.begin_id <= humid_id) && (humid_id < bin.end_id)) {
        loc_id = bin.map + humid_id - bin.begin_id;
        break;
      }
    }

    if (loc_id < smallest_loc_id)
      smallest_loc_id = loc_id;
  }

  printf("Smallest location id: %ld\n", smallest_loc_id);
  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
