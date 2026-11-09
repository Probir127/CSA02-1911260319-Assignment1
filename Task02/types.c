#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdint.h>
int main(void) {
    printf("CHAR_BIT=%d\n", CHAR_BIT);
    printf("char: %zu bytes, %d..%d\n",sizeof(char),CHAR_MIN,CHAR_MAX);
    printf("signed char: %zu bytes, %d..%d\n",sizeof(signed char),SCHAR_MIN,SCHAR_MAX);
    printf("unsigned char: %zu bytes, 0..%u\n",sizeof(unsigned char),(unsigned)UCHAR_MAX);
    printf("short: %zu bytes, %d..%d\n",sizeof(short),SHRT_MIN,SHRT_MAX);
    printf("unsigned short: %zu bytes, 0..%u\n",sizeof(unsigned short),(unsigned)USHRT_MAX);
    printf("int: %zu bytes, %d..%d\n",sizeof(int),INT_MIN,INT_MAX);
    printf("unsigned int: %zu bytes, 0..%u\n",sizeof(unsigned),UINT_MAX);
    printf("long: %zu bytes, %ld..%ld\n",sizeof(long),LONG_MIN,LONG_MAX);
    printf("unsigned long: %zu bytes, 0..%lu\n",sizeof(unsigned long),ULONG_MAX);
    printf("long long: %zu bytes, %lld..%lld\n",sizeof(long long),LLONG_MIN,LLONG_MAX);
    printf("unsigned long long: %zu bytes, 0..%llu\n",sizeof(unsigned long long),ULLONG_MAX);
    printf("float: %zu bytes, normal min=%.9g max=%.9g digits=%d mantissa=%d\n",sizeof(float),(double)FLT_MIN,(double)FLT_MAX,FLT_DIG,FLT_MANT_DIG);
    printf("double: %zu bytes, normal min=%.17g max=%.17g digits=%d mantissa=%d\n",sizeof(double),DBL_MIN,DBL_MAX,DBL_DIG,DBL_MANT_DIG);
    printf("long double: %zu bytes, max=%.9Lg mantissa=%d\n",sizeof(long double),LDBL_MAX,LDBL_MANT_DIG);
    uint16_t meter=UINT16_MAX;
    meter=(uint16_t)(meter+1U);
    printf("16-bit register after 65535 + 1: %u\n",(unsigned)meter);
    return 0;
}
