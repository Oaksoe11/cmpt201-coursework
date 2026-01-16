#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char *buffer = NULL;
  size_t bufsize = 0;

  printf("Enter programs to run.\n");

  while (1) {
    printf("> ");
    fflush(stdout);

    ssize_t len = getline(&buffer, &bufsize, stdin);
    if (len == -1) {
      perror("getline");
      break;
    }

    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
      execl(buffer, buffer, (char *)NULL);

      perror("Exec failure");
      exit(EXIT_FAILURE);
    } else {
      waitpid(pid, NULL, 0);
    }
  }

  free(buffer);
  return 0;
}
