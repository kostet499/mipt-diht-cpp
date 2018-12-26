#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int
main (int argc, char **argv)
{
  int N = (int) strtol(argv[1], NULL, 0);
  pid_t pid = 0;
  for (int counter = 1; counter < N; ++counter)
  {
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
      fprintf(stdout, "%d ", counter);
      exit(0);
    }
    if (pid > 0)
    {
      int status;
      waitpid(pid, &status, 0);
    }
  }
  printf("%d\n", N);
  return 0;
}