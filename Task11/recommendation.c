#include <stdio.h>
#include <limits.h>
#include "../common/billing.h"
int main(void) {
    money_t one=bill_for_units(65535).total_paise;
    money_t district=one*800000LL;
    printf("Maximum monthly bill=%lld paise\nDistrict bound=%lld paise\n",one,district);
    printf("long long maximum=%lld; bound fits=%s\n",LLONG_MAX,district<=LLONG_MAX?"yes":"no");
    puts("Use checked integer paise, a confirmed rollover flag, and one tariff definition.");
    return 0;
}
