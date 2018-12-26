#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


typedef struct Item
{
  INT value;
  UINT next_pointer;
} item_t;


UINT
read_item (item_t *item, HANDLE file_handle);


int
main (int argc, char *argv[])
{
  INT exit_code = 0, in_fd = -1;
  item_t *item = (item_t *) malloc(sizeof(item_t));

  if (argc < 2)
  {
    exit_code = 1;
    goto finalize;
  }

  HANDLE file_handle = CreateFileA(
      argv[1],
      GENERIC_READ,
      FILE_SHARE_READ,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL
  );
  if (file_handle == INVALID_HANDLE_VALUE)
  {
    exit_code = 1;
    goto finalize;
  }

  BOOL empty = TRUE;
  while (read_item(item, file_handle))
  {
    LARGE_INTEGER dist_to_move;
    dist_to_move.QuadPart = item->next_pointer;
    empty = FALSE;
    SetFilePointerEx(
        file_handle,
        dist_to_move,
        NULL,
        FILE_BEGIN
    );
    printf("%d ", (int) item->value);
  }
  if (!empty)
  {
    printf("%d ", (int) item->value);
  }
finalize:
  CloseHandle(file_handle);
  free(item);
  return exit_code;
}


UINT
read_item (item_t *item, HANDLE file_handle)
{
  DWORD bytes_read;
  ReadFile(
      file_handle,
      item,
      sizeof(item_t),
      &bytes_read,
      NULL);
  return item->next_pointer;
}
