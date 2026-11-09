/* Deliberately incorrect. Compile with -Werror=format; do not execute. */
#include <stdio.h>
int main(void) {
    long amount=2147483648L;
    printf("%d\n",amount);
    return 0;
}
