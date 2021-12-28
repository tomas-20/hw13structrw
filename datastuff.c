#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct pop_entry {
  int year;
  int population;
  char boro[15];
};

int char_count(char *string, char chr) {
  int char_count = 0;
  while (*string) {
    if (*string == chr) {
      char_count ++;
    }
    string ++;
  }
  return char_count;
}

void fill_boros(char **boros, char *header) {
  strsep(&header, ",");
  while (header) {
    *boros = strsep(&header, ",");
    boros ++;
  }
}

struct pop_entry *fill_some_entries(struct pop_entry *entries, char **boros, char *line) {
  int year = atoi(strsep(&line, ","));
  while (line) {
    entries->year = year;
    entries->population = atoi(strsep(&line, ","));
    strncpy(entries->boro, *boros, 15);
    entries ++;
    boros ++;
  }
  return entries;
}

int main() {
  char header[] = "Year,Manhattan,Brooklyn,Queens,Bronx,Staten Island";
  int boro_count = char_count(header, ',');
  char *boros[boro_count];
  fill_boros(boros, header);

  for (int i = 0; i < boro_count; i ++) {
    printf("%s\n", boros[i]);
  }

  char line[] = "1790,33131,4549,6159,1781,3827";
  struct pop_entry entries[boro_count];
  fill_some_entries(entries, boros, line);

  for (int i = 0; i < boro_count; i ++) {
    printf("year: %d population: %d boro: %s\n", entries[i].year, entries[i].population, entries[i].boro);
  }
}
