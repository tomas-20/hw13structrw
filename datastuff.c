#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
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

int get_file_size(char *path) {
  struct stat file_stats;
  stat(path, &file_stats);
  return file_stats.st_size;
}

void csv_to_data(char *csv_path, char *data_path) {
  char mesa[get_file_size(csv_path) / sizeof (char)];
  int csv_file = open(csv_path, O_RDONLY);
  read(csv_file, mesa, sizeof mesa);
  close(csv_file);

  char *table = mesa;
  int line_count = char_count(table, '\n') - 1;
  char *header = strsep(&table, "\n");
  int boro_count = char_count(header, ',');
  char *boros[boro_count];
  fill_boros(boros, header);
  struct pop_entry entries[line_count * boro_count];
  fill_entries(entries, boros, table);

  int data_file = open(data_path, O_CREAT | O_WRONLY, 0644);
  write(data_file, entries, sizeof entries);
  close(data_file);
}

void show_entry(struct pop_entry *entry) {
  printf("year: %d boro: %s pop: %d\n", entry->year, entry->boro, entry->population);
}

void show_data(char *data_path) {
  int entry_count = get_file_size(data_path) / sizeof (struct pop_entry);
  struct pop_entry entries[entry_count];
  int data_file = open(data_path, O_RDONLY);
  read(data_file, entries, sizeof entries);
  close(data_file);

  for (int i = 0; i < entry_count; i ++) {
    printf("%d: ", i);
    show_entry(entries + i);
  }
}

void read_csv() {
  csv_to_data("statistics.csv", "statistics.data");
}

void read_data() {
  show_data("statistics.data");
}

int main() {
  read_csv();
  read_data();
}
