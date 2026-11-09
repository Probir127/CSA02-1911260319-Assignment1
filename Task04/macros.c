#include <stdio.h>
#define BAD_SQUARE(x) x*x
#define SQUARE(x) ((x)*(x))
#define MAX(a,b) (((a)>(b))?(a):(b))
/* Whole non-negative rupees only; caller must check multiplication range. */
#define RUPEES_TO_PAISE(r) ((r)*100LL)
static inline int square_int(int x) { return x*x; }
static inline int max_int(int a,int b) { return a>b?a:b; }
int main(void) {
    int a=3, i=5, j=3;
    printf("BAD_SQUARE(a+1)=%d; SQUARE(a+1)=%d\n",BAD_SQUARE(a+1),SQUARE(a+1));
    int result=MAX(i++,j); /* Condition and selected arm are sequenced. */
    printf("MAX(i++,j)=%d; i after=%d\n",result,i);
    i=5;result=max_int(i++,j);
    printf("max_int(i++,j)=%d; i after=%d\n",result,i);
    printf("square_int(a+1)=%d; 50 rupees=%lld paise\n",square_int(a+1),RUPEES_TO_PAISE(50));
    return 0;
}
