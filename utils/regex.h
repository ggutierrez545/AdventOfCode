#ifndef ADVENTOFCODE2023_REGEX_H
#define ADVENTOFCODE2023_REGEX_H
#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>
#include "linked_list.h"

CharNode* apply_regex(char** p, char** s);

#endif //ADVENTOFCODE2023_REGEX_H
