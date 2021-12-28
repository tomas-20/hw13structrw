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

void fill_entries(struct pop_entry *entries, char **boros, char *table) {
  while (table) {
    entries = fill_some_entries(entries, boros, strsep(&table, "\n"));
  }
}

struct pop_entry *get_entries(char *table) {
  int line_count = char_count(table, '\n');
  char *header = strsep(&table, "\n");
  int boro_count = char_count(header, ',');
  char *boros[boro_count];
  fill_boros(boros, header);
  struct pop_entry *entries = malloc(sizeof (struct pop_entry[line_count * boro_count]));
  fill_entries(entries, boros, table);
  return entries;
}

int main() {
  char table[] = "Year,Manhattan,Brooklyn,Queens,Bronx,Staten Island\n1790,33131,4549,6159,1781,3827\n1800,60515,5740,6642,1755,4563\n1810,96373,8303,7444,2267,5347\n1820,123706,11187,8246,2782,6135\n1830,202589,20535,9049,3023,7082\n1840,312710,47613,14480,5346,10965\n1850,515547,138882,18593,8032,15061\n1860,813669,279122,32903,23593,25492\n1870,942292,419921,45468,37393,33029\n1880,1164673,599495,56559,51980,38991\n1890,1441216,838547,87050,88908,51693\n1900,1850093,1166582,152999,200507,67021\n1910,2331542,1634351,284041,430980,85969\n1920,2284103,2018356,469042,732016,116531\n1930,1867312,2560401,1079129,1265258,158346\n1940,1889924,2698285,1297634,1394711,174441\n1950,1960101,2738175,1550849,1451277,191555\n1960,1698281,2627319,1809578,1424815,221991\n1970,1539233,2602012,1986473,1471701,295443\n1980,1428285,2230936,1891325,1168972,352121\n1990,1487536,2300664,1951598,1203789,378977\n2000,1537195,2465326,2229379,1332650,443728\n2010,1585873,2504700,2230722,1385108,468730";
  struct pop_entry *entries = get_entries(table);

  for (int i = 0; i < 115; i ++) {
    printf("year: %d population: %d boro: %s\n", entries[i].year, entries[i].population, entries[i].boro);
  }
}
