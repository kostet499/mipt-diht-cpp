#include <stdio.h>
#include <stdint.h>

uint16_t
satsum (uint16_t x, uint16_t y);

int main( )
{
    return 0;
}


uint16_t
satsum (uint16_t x, uint16_t y)
{
  uint16_t answer = (uint16_t)(x + y),
      maximum_value = (x > y) ? x : y;
  if (answer < maximum_value)
  {
    answer = ~(uint16_t)0;
  }
  return answer;
}