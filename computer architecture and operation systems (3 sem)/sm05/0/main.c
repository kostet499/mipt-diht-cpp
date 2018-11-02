#include <stdint.h>
#include <stdio.h>

uint32_t A = 5, B = 11, C = 42, D = 4;
uint32_t R;

extern void
calculate();

int 
main()
{
	calculate();
	printf("%u\n", R);
	return 0;
}
