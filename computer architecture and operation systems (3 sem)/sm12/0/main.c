#define _GNU_SOURCE
#include <string.h>

#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


extern void
find_matchings (const char *filename,
                const char *pattern,
                struct _IO_FILE *out);

int
main (int argc, char **argv)
{
  if (argc < 3)
  {
    return 1;
  }
  find_matchings(argv[1], argv[2], stdout);
  return 0;
}

extern void
find_matchings (const char *filename,
                const char *pattern,
                struct _IO_FILE *out)
{
  int fd = open(filename, O_RDONLY);
  struct stat file_stat;
  fstat(fd, &file_stat);
  size_t len = (size_t) file_stat.st_size;
  char *data = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
  char *entry = data;
  size_t diff = 0;
  while (NULL != (
      entry = memmem(entry, len - diff, pattern, strlen(pattern))
      ))
  {
    diff = entry - data;
    printf("%lu ", diff);
    entry++;
  }
  munmap(data, (size_t) file_stat.st_size);
  close(fd);
}