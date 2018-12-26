#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <inttypes.h>
#include <pwd.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


extern int
ls_l (const char *entry_name);

int
main (int argc, char **argv)
{
  if (argc < 2)
  {
    return 1;
  }
  int exit_code = ls_l(argv[1]);
  return exit_code;
}

extern int
print_entry_info (const char *entry_name)
{
  int exit_code = 0;
  struct stat entry_stat;
  if (lstat(entry_name, &entry_stat) != -1)
  {
    char type;
    if (S_ISLNK(entry_stat.st_mode))
    {
      type = 'l';
    }
    else if (S_ISDIR(entry_stat.st_mode))
    {
      type = 'd';
    }
    else
    {
      type = '-';
    }
    printf("%c%c%c%c%c%c%c%c%c%c ",
           type,
           (entry_stat.st_mode & S_IRUSR) ? 'r' : '-',
           (entry_stat.st_mode & S_IWUSR) ? 'w' : '-',
           (entry_stat.st_mode & S_IXUSR) ? 'x' : '-',
           (entry_stat.st_mode & S_IRGRP) ? 'r' : '-',
           (entry_stat.st_mode & S_IWGRP) ? 'w' : '-',
           (entry_stat.st_mode & S_IXGRP) ? 'x' : '-',
           (entry_stat.st_mode & S_IROTH) ? 'r' : '-',
           (entry_stat.st_mode & S_IWOTH) ? 'w' : '-',
           (entry_stat.st_mode & S_IXOTH) ? 'x' : '-');
    printf("%d ", (int) entry_stat.st_nlink);
    printf("%s ", getpwuid(entry_stat.st_uid)->pw_name);
    printf("%s ", getgrgid(entry_stat.st_gid)->gr_name);
    printf("%d ", (int) entry_stat.st_size);
    if (S_ISLNK(entry_stat.st_mode))
    {
      char real_name[NAME_MAX];
      readlink(entry_name, real_name, NAME_MAX);
      printf("%s -> %s\n", entry_name, real_name);
    }
    else
    {
      puts(entry_name);
    }
  }
  else
  {
    exit_code = -1;
    goto finalize;
  }
  finalize:
  return exit_code;
}

extern int
ls_l (const char *entry_name)
{
  struct stat entry_stat;
  if (lstat(entry_name, &entry_stat) != -1)
  {
    int exit_code = 0;
    if (S_ISDIR(entry_stat.st_mode))
    {
      chdir(entry_name);
      DIR *dir = opendir(entry_name);
      struct dirent *entry = NULL;
      while ((entry = readdir(dir)))
      {
        if (entry->d_name[0] == '.')
        {
          continue;
        }
        exit_code |= print_entry_info(entry->d_name);
        if (exit_code)
        {
          closedir(dir);
          goto finalize;
        }
      }
      closedir(dir);
      chdir("..");
    }
    else
    {
      exit_code = print_entry_info(entry_name);
    }
    finalize:
    return exit_code;
  }
  else
  {
    return 1;
  }
}