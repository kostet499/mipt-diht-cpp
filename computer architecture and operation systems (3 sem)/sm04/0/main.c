#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    uint8_t  u8;
    uint16_t u16;
    uint32_t u32;
} values_t;

extern uint64_t
calculate(const values_t *values);

int
main ()
{
  values_t* values = (values_t*)malloc(sizeof(values_t));
  values->u8 = (uint8_t)-1;
  values->u16 = (uint16_t)-1;
  values->u32 = (uint32_t)-1;
  printf(
    "calculate(...) = %lld\n"
    , (long long)calculate(values)
  );
  return 0;
}
