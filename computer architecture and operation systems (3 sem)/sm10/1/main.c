#define _FILE_OFFSET_BITS 64

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


extern void
find_invalid_executables (struct _IO_FILE *istream, struct _IO_FILE *ostream);

int
main (int argc, char **argv)
{
  find_invalid_executables(stdin, stdout);
  return 0;
}

extern bool
is_elf_file (char const *const buffer)
{
  return buffer[0] == '\x7F'
         && buffer[1] == 'E'
         && buffer[2] == 'L'
         && buffer[3] == 'F';
}

extern bool
check_for_shebang (char const *const shebang)
{
  return shebang[0] == '#'
         && shebang[1] == '!';
}

void
switch_cases (char const *const filename,
              struct stat *file_stat,
              struct _IO_FILE *ostream)
{
  FILE *executable_file = fopen(filename, "rb");
  const size_t MAX_LENGTH = 4 + PATH_MAX;
  if (executable_file != NULL)
  {
    char shebang[MAX_LENGTH];
    memset(shebang, '\0', sizeof(shebang));
    fgets(shebang, sizeof(shebang), executable_file);
    char* new_line = memchr(shebang, '\n', sizeof(shebang));
    if (new_line)
    {
      *new_line = '\0';
    }
    bool is_correct_elf = file_stat->st_size > 3
                          && is_elf_file(shebang);
    if (is_correct_elf)
    {
      goto finalize;
    }
    else if (check_for_shebang(shebang))
    {
      if (access(shebang + 2, X_OK) != 0)
      {
        fputs(filename, ostream);
      }
    }
    else
    {
      fputs(filename, ostream);
    }
    finalize:
    fclose(executable_file);
  }
}

extern void
find_invalid_executables (struct _IO_FILE *istream, struct _IO_FILE *ostream)
{
  const size_t MAX_LENGTH = 4 + PATH_MAX;
  char filename[MAX_LENGTH];
  struct stat *file_stat = (struct stat *) malloc(sizeof(struct stat));
  char *new_line = NULL;
  while (fgets(filename, sizeof(filename), istream) != 0)
  {
    new_line = memchr(filename, '\n', sizeof(filename));
    if (new_line)
    {
      *new_line = '\0';
    }
    if (-1 != lstat(filename, file_stat) && 0 == access(filename, X_OK))
    {
      switch_cases(filename, file_stat, ostream);
    }
  }
  free(file_stat);
}
