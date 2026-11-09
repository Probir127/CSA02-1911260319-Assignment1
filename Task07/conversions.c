#include <stdio.h>
#include <limits.h>
#include <math.h>
int main(void) {
    int units=150;
    printf("150/100=%d; (double)150/100=%.2f\n",units/100,(double)units/100);
    int negative=-1;unsigned one=1U;
    printf("-1 < 1U = %d (after unsigned conversion)\n",negative<one);
    printf("Signed-domain comparison corrected = %d\n",negative<0 || (unsigned)negative<one);
    unsigned char small=255;
    printf("unsigned char 255 + 1 = %d (integer promotion)\n",small+1);
    if (LONG_MAX>INT_MAX) {
        long large=(long)INT_MAX+1L;
        printf("(int)(INT_MAX+1L)=%d (implementation-defined in C17)\n",(int)large);
    }
    float f=16777216.0f;
    printf("float 16777216 + 1 = %.0f\n",(double)(f+1.0f));
    /* Non-negative charge; finite/range validation must precede llround. */
    double rupees=1.125;
    printf("llround(1.125*100)=%lld paise (half away from zero)\n",llround(rupees*100));
    return 0;
}
