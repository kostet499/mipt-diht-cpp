#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>


int
main ()
{
  int counter = 1;
  pid_t pid;
  do
  {
    pid = fork();
    if (pid != -1)
    {
      ++counter;
    }
  }
  while (pid == 0);
  if (pid > 0)
  {
    int status;
    waitpid(pid, &status, 0);
  }
  else if (pid == -1)
  {
    printf("%d\n", counter);
  }
  return 0;
}
