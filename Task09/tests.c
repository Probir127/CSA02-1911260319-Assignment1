#include "input.h"
#include <assert.h>
#include <stdio.h>
#include <limits.h>
static money_t array_bill(unsigned units) {
    const unsigned widths[]={100,100,UINT_MAX};
    const money_t rates[]={0,225,450};money_t total=5000;
    for (unsigned i=0;i<3&&units;i++) {
        unsigned take=units<widths[i]?units:widths[i];
        total+=(money_t)take*rates[i];units-=take;
    }
    return total;
}
int main(void) {
    const unsigned u[]={0,99,100,101,150,199,200,201,350,65535};
    const money_t expected[]={5000,5000,5000,5225,16250,27275,27500,27950,95000,29428250};
    for (unsigned i=0;i<sizeof u/sizeof u[0];i++) {
        Bill b=bill_for_units(u[i]);assert(b.total_paise==expected[i]);
        assert(b.total_paise==b.energy_paise+b.fixed_paise);assert(b.energy_paise>=0);
        printf("units=%u expected=%lld actual=%lld PASS\n",u[i],expected[i],b.total_paise);
    }
    for (unsigned i=0;i<=65535;i++) assert(bill_for_units(i).total_paise==array_bill(i));
    unsigned consumed=0;
    assert(consumption_checked(65400,120,true,&consumed)&&consumed==256);
    assert(consumption_checked(65535,0,true,&consumed)&&consumed==1);
    assert(!consumption_checked(200,100,false,&consumed));
    assert(!consumption_checked(100,200,true,&consumed));
    Reading r;
    assert(parse_reading("0001 D 00000 00150 N",&r));
    const char *bad[]={"0001 D hello 150 N","0001 D -1 150 N","0001 D 0 65536 N",
        "0001 X 0 150 N","0001 D 0 150 N trailing","0001 D 999999999999999999999999 2 N"};
    for (unsigned i=0;i<sizeof bad/sizeof bad[0];i++) assert(!parse_reading(bad[i],&r));
    money_t total=0;const unsigned five[]={0,100,150,200,350};
    for (unsigned i=0;i<5;i++) assert(add_money(&total,bill_for_units(five[i]).total_paise));
    assert(total==148750);
    total=LLONG_MAX;assert(!add_money(&total,1));assert(total==LLONG_MAX);
    puts("65,536 array-vs-fixed comparisons; rollover, parser, sum and overflow guards: PASS");
    puts("Five hand-calculated bills total: 148750 paise = Rs 1487.50");
    return 0;
}
