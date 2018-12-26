#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64

#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct Item
{
  int value;
  uint32_t next_pointer;
} item_t;

int
main (int argc, char *argv[])
{
  int exit_code = 0, in_fd = -1;
  item_t *item = (item_t *) malloc(sizeof(item_t));
  if (argc < 2 || (in_fd = open(argv[1], O_RDONLY)) == -1)
  {
    exit_code = 1;
    goto finalize;
  }
  struct stat file_stat;
  fstat(in_fd, &file_stat);
  size_t len = (size_t) file_stat.st_size;
  if (len != 0)
  {
    char *data = mmap(NULL, len, PROT_READ, MAP_PRIVATE, in_fd, 0);
    memcpy(item, data, sizeof(item_t));
    while (item->next_pointer != 0)
    {
      printf("%d ", item->value);
      memcpy(item, data + item->next_pointer, sizeof(item_t));
    }
    printf("%d\n", item->value);
    munmap(data, len);
  }
  finalize:
  close(in_fd);
  free(item);
  return exit_code;
}