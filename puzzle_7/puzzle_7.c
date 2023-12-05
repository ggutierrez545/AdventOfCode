#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>


int main(void) {

  FILE* fptr;
  char* line = NULL;
  size_t len = 0;

  fptr = fopen("example.txt", "r");
  if (fptr == NULL)
    exit(EXIT_FAILURE);

  regex_t reegex;
  // regcomp(&reegex, "\\d+(?=
  regcomp(&reegex, "([A-Z])\\w+", 0);

  while (getline(&line, &len, fptr) != -1) {
    line[strcspn(line, "\n")] = 0;
    printf("%s\n", line);
    size_t nmatch = 1;
    regmatch_t pmatch[2];
    regexec(&reegex, line, nmatch, pmatch, 0);

    char *result;
    result = (char*)malloc(pmatch[0].rm_eo - pmatch[0].rm_so);
    printf("%s", result);
  }

  fclose(fptr);
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
