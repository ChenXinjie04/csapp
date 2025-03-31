#include "csapp.h"
#include <stdio.h>
#define MAXARGS 128

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main(int argc, char **argv) {
  char cmdline[MAXLINE];

  while (1) {
    /* Read */
    printf("> ");
    Fgets(cmdline, MAXLINE, stdin);
    if (feof(stdin)) {
      exit(0);
    }

    /* Evaluate */
    eval(cmdline);
  }
}

/* eval - Evaluate a command line */
void eval(char *cmdline) {
  char *argv[MAXARGS];
  char buf[MAXLINE];
  int bg;
  pid_t pid;

  strcpy(buf, cmdline);
  bg = parseline(buf, argv);
  if (argv[0] == NULL) {
    return;
  }

  if (!builtin_command(argv)) {
    if ((pid = Fork()) == 0) {
      Execve(argv[0], argv, environ);
    }
  }

  if (!bg) {
    int status;
    Waitpid(pid, &status, 0);
  } else {
    printf("%d %s", pid, cmdline);
  }
  return;
}

int buildin_command(char **argv) {
  if (!strcmp(argv[0], "quit")) {
    exit(0);
  }
  if (!strcmp(argv[0], "&")) {
    return 1;
  }
  return 0;
}

/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) {
  char *delim;
  int argc;
  int bg;

  buf[strlen(buf) - 1] = ' ';
  while (buf && (*buf == ' ')) { /* Ignore leading spaces */
    buf++;
  }

  /* Build the argv list */
  argc = 0;
  while ((delim = strchr(buf, ' '))) {
    argv[argc++] = buf;
    *delim = 0;
    buf = delim + 1;
    while (buf && (*buf == ' ')) { /* Ignore spaces */
      buf += 1;
    }
  }
  argv[argc] = NULL;

  if (argc == 0) {
    return 1;
  }

  if ((bg = *argv[argc - 1] == '&') != 0) {
    argv[argc - 1] = NULL;
  }
  return bg;
}
