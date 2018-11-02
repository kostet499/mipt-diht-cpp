#include <stdio.h>

typedef enum 
{
    PlusZero      = 0x00,
    MinusZero     = 0x01,
    PlusInf       = 0xF0,
    MinusInf      = 0xF1,
    PlusRegular   = 0x10,
    MinusRegular  = 0x11,
    PlusDenormal  = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN  = 0x30,
    QuietNaN      = 0x31
} float_class_t;

extern float_class_t
classify(double *value_ptr);

typedef union 
{
	double f;
	struct
	{
		unsigned long long int m : 52;
		unsigned int e : 11;
		unsigned int s : 1;
	};
} union_t;

int main()
{
	union_t uni;
	
	uni.s = 0;
	uni.e = 0;
	uni.m = 0;
	printf("PlusZero: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 0;
	uni.m = 0;
	printf("MinusZero: %d\n", classify(&uni.f));
	
	uni.s = 0;
	uni.e = 2047;
	uni.m = 0;
	printf("PlusInf: %d\n", classify(&uni.f));
	
	uni.s = 1;
	uni.e = 2047;
	uni.m = 0;
	printf("MinusInf: %d\n", classify(&uni.f));
		
	uni.s = 0;
	uni.e = 2047;
	uni.m = 1LL << 51;
	printf("SignalingNaN: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 2047;
	uni.m = 1LL << 51;
	printf("QuietNaN: %d\n", classify(&uni.f));
		
	uni.s = 0;
	uni.e = 0;
	uni.m = 1LL << 51;
	printf("PlusDenormal: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 0;
	uni.m = 1LL << 51;
	printf("MinusDenormal: %d\n", classify(&uni.f));
		
	uni.s = 0;
	uni.e = 1;
	uni.m = 1;
	printf("PlusRegular: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 1;
	uni.m = 1LL << 51;
	printf("MinusRegular: %d\n", classify(&uni.f));
	
	uni.s = 0;
	uni.e = 2047;
	uni.m = 1;
	printf("SignalingNaN: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 2047;
	uni.m = 1;
	printf("QuietNaN: %d\n", classify(&uni.f));
		
	uni.s = 0;
	uni.e = 0;
	uni.m = 1;
	printf("PlusDenormal: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 0;
	uni.m = 1;
	printf("MinusDenormal: %d\n", classify(&uni.f));
		
	uni.s = 0;
	uni.e = 1;
	uni.m = 1;
	printf("PlusRegular: %d\n", classify(&uni.f));
		
	uni.s = 1;
	uni.e = 1;
	uni.m = 1;
	printf("MinusRegular: %d\n", classify(&uni.f));
	
	return 0;
}
