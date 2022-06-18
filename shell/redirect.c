#include "def.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_redirect(char *argv[]) {
  int n = 0, fd;
  /* < */
  n = search_argv(argv, "<");
  if (n > 0) {
    /* file only */
    fd = open(argv[n + 1], O_RDONLY);
    if (fd < 0) {
      perror(argv[n + 1]);
      exit(1);
    }
    close(0);
    dup2(fd, 0);
    close(fd);

    delete_argv(argv, n, 2);
  }

  /* > */
  /* 1> */
  n = search_argv(argv, ">");
  if (!n) {
    n = search_argv(argv, "1>");
  }
  if (n > 0) {
    printf(">\n");
    /* file only */
    fd = open(argv[n + 1], O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
      perror(argv[n + 1]);
      exit(1);
    }
    close(1);
    dup2(fd, 1);
    close(fd);

    delete_argv(argv, n, 2);
  }

  /* 2> */
  n = search_argv(argv, "2>");
  if (n > 0) {
    printf("2>\n");
    /* file only */
    fd = open(argv[n + 1], O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
      perror(argv[n + 1]);
      exit(1);
    }
    close(2);
    dup2(fd, 2);
    close(fd);

    delete_argv(argv, n, 2);
  }

  /* 2>&1 */
  n = search_argv(argv, "2>&1");
  if (n > 0) {
    printf("2>&1\n");
    /* file only */
    fd = open(argv[n + 1], O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
      perror(argv[n + 1]);
      exit(1);
    }
    close(1);
    close(2);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);

    delete_argv(argv, n, 2);
  }
}
