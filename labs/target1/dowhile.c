#include <stdio.h>

int fibonacci(int n) {
  int result = 1;
  int t = n > 1;
  if (!t)
    goto done;
loop:
  result *= n;
test:
  if (n > 1) goto loop;
done:
  return result;
}
