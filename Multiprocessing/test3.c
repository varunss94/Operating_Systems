
//  sample test3.c



#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid = getpid();
  printf("Running program test3 in process %d\n", (int)pid);
  if ((int)pid % 2)
    printf("T3: PID %d is odd\n", (int)pid);
  else
    printf("T3: PID %d is even\n", (int)pid);
  return 0;
}
