#include "dbg.h"
#include <stdlib.h>

void test_debug() {
  debug("I have Brown Hair.");
  
  debug("I am %d years old.", 37);
}

void test_log_error() {
  log_err("I believe everything is broken.");
  log_err("There are %d problems in %s.", 0, "space");
}

void test_log_warn() {
  log_warn("You can safely ignore this.");
  log_warn("Maybe consider looking at: %s.", "/etc/passwd");
}

void test_log_info() {
  log_info("Well I did something mundane.");
  log_info("It happened %f times today.", 1.3f);
}

int test_check(char *filename) {
  FILE *input = NULL;
  char *block = NULL;
  
  block = malloc(100);
  check_mem(block);
  
  input = fopen(filename, "r");
  check(input, "Fail to open %s", filename);

  free(block);
  fclose(input);
  return 0;
error:
  if (block) free(block);
  if (input) fclose(input);
  return -1;
}

int test_sentinel(int code) {
  switch(code) {
    case 0:
      log_info("It worked.");
      break;
    default:
      sentinel("I shouldn't run.");
  }
  return 0;
error:
  return -1;
}

int test_check_mem() {
  char *test = NULL;
  check_mem(test);

  free(test);
  return 0;
error:
  return -1;
}

int test_check_debug() {
  int i = 0;
  check_debug(i != 0, "Oops, I was 0.");

  return 0;
error:
  return -1;
}

int main(int argc, char *argv[]) {
  check(argc == 2, "Need an argument.");
  
  test_debug();
  test_log_error();
  test_log_warn();
  test_log_info();
  check(test_check("ex19.c") == 0, "failed with ex19.c");
  check(test_sentinel(0) == 0, "test_sentinel failed.");
  check(test_sentinel(1) == -1, "test_sentinel failed.");
  check(test_check_mem() == -1, "test_check_mem failed.");
  check(test_check_debug() == -1, "test_check_debug failed.");
  

  return 0;
error:
  return 1;
}