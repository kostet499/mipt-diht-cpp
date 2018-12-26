#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int
main (argc, argv)
    int argc;
    char** argv;
{
  int exit_code = 0;
  if (argc < 4)
  {
    exit_code = 3;
    goto finalize;
  }
  int input_fd = open(argv[1], O_RDONLY);
  if (input_fd == -1)
  {
    exit_code = 1;
    goto finalize;
  }
  // 0640 ~ -rw-r--r--
  int only_digits_output_fd = open(argv[2], O_WRONLY | O_CREAT, 0640),
    other_chars_output_fd = open(argv[3], O_WRONLY | O_CREAT, 0640);
  if (only_digits_output_fd == -1 || other_chars_output_fd == -1)
  {
    exit_code = 2;
    goto finalize;
  }
  ssize_t in;
  int output_fd;
  char input_byte;
  while ((in = read(input_fd, &input_byte, sizeof(input_byte))) > 0)
  {
    int input_byte_ascii = (int)input_byte;
    if (input_byte_ascii >= 48 && input_byte_ascii <= 57)
    {
      output_fd = only_digits_output_fd;
    }
    else
    {
      output_fd = other_chars_output_fd;
    }
    if (write(output_fd, &input_byte, sizeof(input_byte)) == -1)
    {
      exit_code = 3;
      goto finalize;
    }
  }
  if (in != 0)
  {
    exit_code = 3;
    goto finalize;
  }
finalize:
  close(input_fd);
  close(only_digits_output_fd);
  close(other_chars_output_fd);
  return exit_code;
}