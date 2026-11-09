#include "smart.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
int main(int argc,char **argv) {
    assert(argc==2);Tariff t;assert(load_tariff(argv[1],&t));
    Account a={0,0,0};uint64_t energy,total;
    assert(apply_interval(&a,&t,100000,71));
    assert(apply_interval(&a,&t,50000,72));
    assert(apply_interval(&a,&t,50000,73));
    assert(apply_interval(&a,&t,150000,74));
    assert(finish_bill(&a,&t,&energy,&total)&&energy==108000&&total==113000);
    printf("350 kWh mixed/slab-crossing test: energy=%" PRIu64 " total=%" PRIu64 " paise PASS\n",energy,total);
    const unsigned slots[]={71,72,87,88};const uint64_t expected[]={450,540,540,450};
    for (unsigned i=0;i<4;i++) {
        a=(Account){200000,0,0};assert(apply_interval(&a,&t,1000,slots[i]));
        assert(finish_bill(&a,&t,&energy,&total)&&energy==expected[i]);
        printf("slot=%u (start %02u:%02u) energy=%" PRIu64 " paise PASS\n",slots[i],slots[i]/4,slots[i]%4*15,energy);
    }
    a=(Account){200000,0,0};assert(apply_interval(&a,&t,1,0));assert(apply_interval(&a,&t,1,1));
    assert(finish_bill(&a,&t,&energy,&total)&&energy==1);
    puts("Two 0.45-paisa intervals -> 1 paisa after one final rounding: PASS");
    a=(Account){200000,UINT64_MAX-1,0};Account before=a;
    assert(!apply_interval(&a,&t,1000,0));assert(a.charge_numerator==before.charge_numerator&&a.used_wh==before.used_wh);
    assert(!apply_interval(&a,&t,t.max_interval_wh+1,0));
    puts("Overflow and over-limit rejection leave account unchanged: PASS");
    a=(Account){0,0,0};
    for (unsigned day=0;day<365;day++) for (unsigned slot=0;slot<96;slot++)
        assert(apply_interval(&a,&t,1000,slot));
    assert(a.intervals==35040&&a.used_wh==35040000);
    printf("Annual interval traversal=%" PRIu64 " count; total=%" PRIu64 " Wh PASS\n",a.intervals,a.used_wh);
    return 0;
}
