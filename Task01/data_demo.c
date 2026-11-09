#include <stdio.h>
#include "../common/billing.h"
int main(void) {
    const char household[]="00001234";
    const char category='D';
    meter_t previous=1200, current=1530;
    unsigned units=0;
    if (!consumption_checked(previous,current,false,&units)) return 1;
    Bill b=bill_for_units(units);
    printf("Household=%s category=%c previous=%u current=%u units=%u\n",
           household,category,(unsigned)previous,(unsigned)current,units);
    printf("Energy: ");print_money(b.energy_paise);
    printf("\nFixed: ");print_money(b.fixed_paise);
    printf("\nTotal: ");print_money(b.total_paise);putchar('\n');
    printf("Nonnegative and sum invariant: %s\n",
           b.total_paise>=0 && b.total_paise==b.energy_paise+b.fixed_paise?"PASS":"FAIL");
    return 0;
}
