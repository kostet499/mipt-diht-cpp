#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


extern void
manage_symlinks (struct _IO_FILE *in, struct _IO_FILE *out);

int
main (int argc, char** argv)
{
  manage_symlinks(stdin, stdout);
  return 0;
}

extern void
manage_symlinks (struct _IO_FILE *in, struct _IO_FILE *out)
{
  const char* PREFIX = "link_to_";
  char filename[PATH_MAX];
  struct stat *file_stat = (struct stat*)malloc(sizeof(struct stat));
  char *new_line = NULL;
  while (fgets(filename, sizeof(filename), in) != 0)
  {
    new_line = memchr(filename, '\n', sizeof(filename));
    if (new_line)
    {
      *new_line = '\0';
    }
    if (lstat(filename, file_stat) != -1)
    {
      if (S_ISLNK(file_stat->st_mode))
      {
        char* real_path_str = (char*)malloc(PATH_MAX * sizeof(char));
        real_path_str = realpath(filename, real_path_str);
        printf("%s\n", real_path_str);
      }
      else if (S_ISREG(file_stat->st_mode))
      {
        char link_name[strlen(PREFIX) + PATH_MAX];
        memcpy(link_name, PREFIX, strlen(PREFIX));
        memcpy(link_name + strlen(PREFIX), filename, sizeof(filename));
        symlink(filename, link_name);
      }
    }
  }
  free(file_stat);
}