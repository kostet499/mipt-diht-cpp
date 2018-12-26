#define _FILE_OFFSET_BITS 64

#include <dirent.h>
#include <fcntl.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


extern void
show_dirs (const char *dirname, int year, int month, int day);

int
main (int argc, char **argv)
{
  if (argc < 5)
  {
    exit(1);
  }
  int32_t year = (int32_t) strtol(argv[2], NULL, 10),
      month = (int32_t) strtol(argv[3], NULL, 10),
      day = (int32_t) strtol(argv[4], NULL, 10);
  show_dirs(argv[1], year, month, day);
  return 0;
}

extern void
show_dirs (const char *dirname, int year, int month, int day)
{
  int dir_fd = open(dirname, O_RDONLY | O_DIRECTORY);
  DIR *dir = fdopendir(dir_fd);
  struct tm limit_tm;
  memset(&limit_tm, 0, sizeof(limit_tm));
  limit_tm.tm_year = year - 1900;
  limit_tm.tm_mon = month - 1;
  limit_tm.tm_mday = day;
  limit_tm.tm_isdst = -1;
  time_t limit_time = mktime(&limit_tm);
  struct dirent *entry = NULL;
  struct stat *dir_stat = (struct stat *) malloc(sizeof(struct stat));
  while ((entry = readdir(dir)))
  {
    fstatat(dir_fd, entry->d_name, dir_stat, 0);
    if (dir_stat->st_mtime >= limit_time)
    {
      printf("%s\n", entry->d_name);
    }
  }
  free(dir_stat);
  closedir(dir);
}
