#include <stdlib.h>

int main() {
  char *foo = malloc(2);
  free(foo);
  free(foo);
}