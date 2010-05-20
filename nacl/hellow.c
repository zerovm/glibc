
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  const char *msg = "Hello world via write()\n";
  write(1, msg, strlen(msg));
  printf("Hello world via printf(), %i\n", 1234);
  return 0;
}
