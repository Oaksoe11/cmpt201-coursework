#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

void add_history(char *history[], int *count, char *line) {
  if (*count < HISTORY_SIZE) {
    history[*count] = line;
    (*count)++;
  } else {
    free(history[0]);
    for (int i = 1; i < HISTORY_SIZE; i++) {
      history[i - 1] = history[i];
    }
    history[HISTORY_SIZE - 1] = line;
  }
}

void print_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    printf("%s", history[i]);
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {NULL};
  int count = 0;

  char *line = NULL;
  size_t size = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    ssize_t nread = getline(&line, &size, stdin);
    if (nread == -1) {
      break;
    }

    char *stored = strdup(line);
    if (stored == NULL) {
      perror("strdup");
      break;
    }

    add_history(history, &count, stored);

    if (strcmp(stored, "print\n") == 0) {
      print_history(history, count);
    }
  }

  for (int i = 0; i < count; i++) {
    free(history[i]);
  }
  free(line);

  return 0;
}
