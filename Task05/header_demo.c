#include <stdio.h>
#include "../common/billing.h"
#include "../common/billing.h" /* Repeated inclusion is harmless with guards. */
int main(void) {
    Bill b=bill_for_units(150);
    printf("Header guards work; total=");print_money(b.total_paise);putchar('\n');
    return 0;
}
