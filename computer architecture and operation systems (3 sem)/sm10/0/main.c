#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


extern uint64_t
calc_total_size (struct _IO_FILE * io_file);

int
main (int argc, char** argv)
{
  printf("%"PRIu64"\n", calc_total_size(stdin));
  return 0;
}

extern uint64_t
calc_total_size (struct _IO_FILE * io_file)
{
  char filename[PATH_MAX];
  struct stat *file_stat = (struct stat*)malloc(sizeof(struct stat));
  char *new_line = NULL;
  uint64_t total_size = 0LL;
  while (fgets(filename, sizeof(filename), io_file) != 0)
  {
    new_line = memchr(filename, '\n', sizeof(filename));
    if (new_line)
    {
      *new_line = '\0';
    }
    if (lstat(filename, file_stat) != -1)
    {
      if (S_ISREG(file_stat->st_mode))
      {
        total_size += file_stat->st_size;
      }
    }
  }
  free(file_stat);
  return total_size;
}