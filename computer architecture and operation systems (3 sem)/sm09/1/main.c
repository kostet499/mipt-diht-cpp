#include <fcntl.h>
#include <unistd.h>

#include <malloc.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>


typedef struct Item
{
  int value;
  uint32_t next_pointer;
} item_t;


uint32_t
read_item (item_t *item, int fd);


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
  bool empty = true;
  while (read_item(item, in_fd))
  {
    empty = false;
    lseek(in_fd, item->next_pointer, SEEK_SET);
    printf("%d ", item->value);
  }
  if (!empty)
  {
    printf("%d ", item->value);
  }
finalize:
  close(in_fd);
  free(item);
  return exit_code;
}


uint32_t
read_item (item_t *item, int fd)
{
  read(fd, item, sizeof(item_t));
  return item->next_pointer;
}
