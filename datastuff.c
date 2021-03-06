#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#define CSV_PATH "statistics.csv"
#define DATA_PATH "statistics.data"

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
  char *table = mesa;

  int csv_file = open(csv_path, O_RDONLY);
  read(csv_file, mesa, sizeof mesa);
  close(csv_file);

  int line_count = char_count(table, '\n') - 1;
  char *header = strsep(&table, "\n");
  int boro_count = char_count(header, ',');
  char *boros[boro_count];
  fill_boros(boros, header);
  struct pop_entry entries[line_count * boro_count];
  fill_entries(entries, boros, table);

  int data_file = open(data_path, O_WRONLY | O_TRUNC | O_CREAT , 0644);
  write(data_file, entries, sizeof entries);
  close(data_file);

  printf("Imported entries from %s\n", csv_path);
}

void show_entry(struct pop_entry *entry) {
  printf("year: %d boro: %s pop: %d\n", entry->year, entry->boro, entry->population);
}

void show_data(char *path) {
  int entry_count = get_file_size(path) / sizeof (struct pop_entry);
  struct pop_entry entries[entry_count];
  int file = open(path, O_RDONLY);
  read(file, entries, sizeof entries);
  close(file);

  for (int i = 0; i < entry_count; i ++) {
    printf("%d: ", i);
    show_entry(entries + i);
  }
}

void fill_entry(struct pop_entry *entry) {
  char input[1000];
  printf("year: ");
  fgets(input, 1000, stdin);
  entry->year = atoi(input);
  printf("boro: ");
  fgets(input, 1000, stdin);
  strtok(input, "\n");
  strncpy(entry->boro, input, 15);
  printf("pop: ");
  fgets(input, 1000, stdin);
  entry->population = atoi(input);
}

void add_entry(char *path) {
  struct pop_entry entry;
  fill_entry(&entry);

  int file = open(path, O_WRONLY | O_APPEND);
  write(file, &entry, sizeof entry);
  close(file);

  printf("Added entry: ");
  show_entry(&entry);
}

void update_entry(char *path) {
  printf("Entry to update: ");
  char input[1000];
  fgets(input, 1000, stdin);
  int index = atoi(input);
  struct pop_entry entry;
  fill_entry(&entry);

  int file = open(path, O_WRONLY);
  lseek(file, sizeof (struct pop_entry[index]), SEEK_SET);
  write(file, &entry, sizeof entry);
  printf("Updated entry %d to: ", index);
  show_entry(&entry);
}

int main(int argc, char **argv) {
  if (argc > 1) {
    char *flag = argv[1];
    if (!strcmp(flag, "-read_csv")) {
      csv_to_data(CSV_PATH, DATA_PATH);
    }
    else if (!strcmp(flag, "-read_data")) {
      show_data(DATA_PATH);
    }
    else if (!strcmp(flag, "-add_data")) {
      add_entry(DATA_PATH);
    }
    else if (!strcmp(flag, "-update_data")) {
      update_entry(DATA_PATH);
    }
  }
}
