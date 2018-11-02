#include <assert.h>
#include <stdint.h>
#include <stdio.h>

extern int
check_int (uint32_t u32); /* retval: 0 - false, 1 - true */

int
main ()
{
	uint32_t u32 = 4388607;
	for (; u32 < 18388607; u32++)
	{
		float f = u32;
		uint64_t y = f;
		if (check_int(u32))
		{
			assert(u32==y);
		}	
		else
		{
			printf("FAIL! -> %d\n", u32);
			
		}
	}
	u32 = 0;
	printf("check_int(%d)=%d\n", u32, check_int(u32));
	
	u32 = 228;
	printf("check_int(%d)=%d\n", u32, check_int(u32));
	
	u32 = 8388607;
	printf("check_int(%d)=%d\n", u32, check_int(u32));
	
	u32 = 8388608;
	printf("check_int(%d)=%d\n", u32, check_int(u32));
	
	u32 = 8388609;
	printf("check_int(%d)=%d\n", u32, check_int(u32));
	
	return 0;
}
