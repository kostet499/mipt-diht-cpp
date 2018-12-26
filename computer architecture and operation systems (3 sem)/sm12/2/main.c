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


typedef char byte;


extern void
write_mmap_file (byte *map, ssize_t N, ssize_t W);

int
main (int argc, char **argv)
{
  int fd = open(argv[1], O_RDWR | O_CREAT, (mode_t) 0600);
  ssize_t N = (ssize_t) strtol(argv[2], NULL, 10);
  ssize_t W = (ssize_t) strtol(argv[3], NULL, 10);
  size_t out_size = (size_t) (N * (N * W + 1));
  ftruncate(fd, out_size);
  byte *map = mmap(
      NULL,
      out_size,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      fd,
      0
  );
  write_mmap_file(map, N, W);
  munmap(map, out_size);
  close(fd);
  return 0;
}


extern __off64_t
compute_offset (ssize_t i, ssize_t j, ssize_t N, ssize_t W)
{
  return (__off64_t) (j * W + i * (W * N + 1));
}

extern char *
convert_to_cell (uint32_t counter, ssize_t W)
{
  byte *buffer = (byte *) malloc(W * sizeof(byte));
  for (ssize_t position = W - 1; position >= 0; --position)
  {
    if (counter > 0)
    {
      buffer[position] = (byte) (48 + counter % 10);
      counter /= 10;
    }
    else
    {
      buffer[position] = ' ';
    }
  }
  return buffer;
}

extern void
write_mmap (void *map, __off64_t offset, const void *src, ssize_t src_len)
{
  memcpy(map + offset, src, (size_t) src_len);
}

extern void
write_mmap_int (void *map, __off64_t offset, uint32_t counter, ssize_t W)
{
  byte *buffer = convert_to_cell(counter, W);
  write_mmap(map, offset, buffer, W);
  free(buffer);
}

extern void
write_mmap_file (byte *map, ssize_t N, ssize_t W)
{
  uint32_t counter = 0;
  for (ssize_t k = 0; k < N; ++k)
  {
    for (ssize_t i = k; i < N - k - 1; ++i)
    {
      __off64_t offset = compute_offset(
          (size_t) k,
          (size_t) i,
          N,
          W);
      write_mmap_int(map, offset, ++counter, W);
    }
    for (ssize_t i = k; i < N - k - 1; ++i)
    {
      __off64_t offset = compute_offset(
          (size_t) i,
          (size_t) (N - k - 1),
          N,
          W);
      write_mmap_int(map, offset, ++counter, W);
    }
    for (ssize_t i = k; i < N - k - 1; ++i)
    {
      __off64_t offset = compute_offset(
          (size_t) (N - k - 1),
          (size_t) (N - i - 1),
          N,
          W);
      write_mmap_int(map, offset, ++counter, W);
    }
    for (ssize_t i = k; i < N - k - 1; ++i)
    {
      __off64_t offset = compute_offset(
          (size_t) (N - i - 1),
          (size_t) k,
          N,
          W);
      write_mmap_int(map, offset, ++counter, W);
    }
  }
  if (N % 2 == 1)
  {
    __off64_t offset = compute_offset(
        (size_t) (N / 2),
        (size_t) (N / 2),
        N,
        W);
    write_mmap_int(map, offset, ++counter, W);
  }
  for (ssize_t i = N * W; i < N * (N * W + 1); i += N * W + 1)
  {
    map[i] = '\n';
  }
}
