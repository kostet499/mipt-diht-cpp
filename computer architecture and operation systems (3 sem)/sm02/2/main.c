#include <stdio.h>


enum
{
  ANTI_ASCII_TEMPLATE = ~0x7f
};


int
execute (unsigned int *total_ascii, unsigned int *total_non_ascii);


int
main ()
{
  unsigned int total_ascii = 0
  , total_non_ascii = 0;
  int status_code = execute(&total_ascii, &total_non_ascii);
  printf("%d %d\n", total_ascii, total_non_ascii);
  return status_code;
}


// Check if 'value' is like 10xxxxxx
int
check_for_continuation_byte (int value)
{
  return ((value >> 6) & 2) == 2;
}


// Check if 'value' is ASCII character
int
check_ascii (int value)
{
  return ((value & ANTI_ASCII_TEMPLATE) == 0);
}


// Returns amount of continuation bytes of utf-8 character.
// If 'value' is not like 1110xxxx (or with other amount of
// 1s at the beginning) then return -1.
int
get_continuation_bytes (int value)
{
  for (int bit_shift = 1; bit_shift < 6; bit_shift++)
  {
    if ((value >> (6 - bit_shift)) == (1 << (bit_shift + 2)) - 2)
    {
      return bit_shift;
    }
  }
  return -1;
}


int
execute (unsigned int *total_ascii, unsigned int *total_non_ascii)
{
  int input = 0;
  while ((input = getchar()) != EOF)
  {
    if (check_ascii(input))
    {
      (*total_ascii)++;
      continue;
    }
    int continuation_bytes_amt = get_continuation_bytes(input);
    if (continuation_bytes_amt == -1)
    {
      return 1;
    }
    while (continuation_bytes_amt != 0)
    {
      input = getchar();
      if (!check_for_continuation_byte(input))
      {
        return 1;
      }
      continuation_bytes_amt--;
    }
    (*total_non_ascii)++;
  }
  return 0;
}
