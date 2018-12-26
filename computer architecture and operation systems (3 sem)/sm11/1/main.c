#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>


extern struct tm
parse_tm (const char *source);

extern int64_t
timediff (struct tm from_tm, struct tm to_tm);


int
main (int argc, char *argv[])
{
  const uint8_t TIME_FMT_SIZE = 42;
  char from_str[TIME_FMT_SIZE];
  char to_str[TIME_FMT_SIZE];
  bool ready = false;
  while (fgets(to_str, TIME_FMT_SIZE, stdin) != NULL)
  {
    if (ready)
    {
      struct tm from_tm = parse_tm(from_str);
      struct tm to_tm = parse_tm(to_str);
      printf("%"PRId64"\n", timediff(from_tm, to_tm));
      memcpy(from_str, to_str, TIME_FMT_SIZE);
    }
    else
    {
      ready = true;
      memcpy(from_str, to_str, TIME_FMT_SIZE);
    }
  }
  return 0;
}


extern struct tm
parse_tm (const char *source)
{
  struct tm source_tm;
  memset(&source_tm, 0, sizeof(source_tm));
  sscanf(source,
         "%d-%d-%d %d:%d",
         &source_tm.tm_year,
         &source_tm.tm_mon,
         &source_tm.tm_mday,
         &source_tm.tm_hour,
         &source_tm.tm_min);
  source_tm.tm_year -= 1900;
  source_tm.tm_mon -= 1;
  source_tm.tm_isdst = -1;
  return source_tm;
}

extern int64_t
timediff (struct tm from_tm, struct tm to_tm)
{
  int64_t from_time = (int64_t) mktime(&from_tm);
  int64_t to_time = (int64_t) mktime(&to_tm);
  return (to_time - from_time) / 60;
}

