#include "smart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
static bool add(uint64_t x,uint64_t y,uint64_t *out) {
    if (x>UINT64_MAX-y) return false;
    *out=x+y;return true;
}
static bool mul(uint64_t x,uint64_t y,uint64_t *out) {
    if (y && x>UINT64_MAX/y) return false;
    *out=x*y;return true;
}
bool load_tariff(const char *path,Tariff *t) {
    const char *names[]={"slab1_wh","slab2_wh","rate1_paise","rate2_paise","rate3_paise",
        "fixed_paise","peak_start","peak_end","peak_percent","max_interval_wh"};
    uint64_t values[10]={0};unsigned seen=0;char line[160];
    FILE *f=fopen(path,"r");if (!f) return false;
    bool ok=true;
    while (fgets(line,sizeof line,f)) {
        char key[40],value[40],extra;
        if (!strchr(line,'\n')&&!feof(f)) { ok=false;break; }
        if (sscanf(line,"%39[^=]=%39s %c",key,value,&extra)!=2) { ok=false;break; }
        for (const char *p=value;*p;p++) if (*p<'0'||*p>'9') ok=false;
        if (!ok) break;
        unsigned i;for (i=0;i<10&&strcmp(key,names[i]);i++) {}
        if (i==10 || (seen&(1U<<i))) { ok=false;break; }
        errno=0;char *end=NULL;unsigned long long number=strtoull(value,&end,10);
        if (errno==ERANGE || *end || number>UINT64_MAX) { ok=false;break; }
        values[i]=(uint64_t)number;seen|=1U<<i;
    }
    if (ferror(f)) ok=false;
    fclose(f);
    if (!ok || seen!=1023U) return false;
    if (!values[0]||values[0]>=values[1]||values[1]>1000000000ULL ||
        values[2]>100000 || values[3]>100000 || values[4]>100000 ||
        values[5]>1000000 || values[6]>=values[7] || values[7]>24 ||
        values[8]>100 || !values[9] || values[9]>1000000) return false;
    *t=(Tariff){values[0],values[1],values[2],values[3],values[4],values[5],
                values[6],values[7],values[8],values[9]};
    return true;
}
bool apply_interval(Account *a,const Tariff *t,uint64_t delta_wh,unsigned slot) {
    if (!a || !t || slot>95 || delta_wh>t->max_interval_wh) return false;
    Account next=*a;uint64_t remaining=delta_wh;
    unsigned hour=slot/4;
    uint64_t factor=100+((hour>=t->peak_start&&hour<t->peak_end)?t->peak_percent:0);
    while (remaining) {
        uint64_t rate,capacity;
        if (next.used_wh<t->slab1_wh) { rate=t->rate1;capacity=t->slab1_wh-next.used_wh; }
        else if (next.used_wh<t->slab2_wh) { rate=t->rate2;capacity=t->slab2_wh-next.used_wh; }
        else { rate=t->rate3;capacity=remaining; }
        uint64_t take=remaining<capacity?remaining:capacity,term;
        if (!mul(take,rate,&term)||!mul(term,factor,&term)||
            !add(next.charge_numerator,term,&next.charge_numerator)||
            !add(next.used_wh,take,&next.used_wh)) return false;
        remaining-=take;
    }
    if (!add(next.intervals,1,&next.intervals)) return false;
    *a=next;return true; /* Commit only after every checked operation succeeds. */
}
bool finish_bill(const Account *a,const Tariff *t,uint64_t *energy,uint64_t *total) {
    uint64_t rounded=a->charge_numerator/100000;
    if (a->charge_numerator%100000>=50000 && !add(rounded,1,&rounded)) return false;
    if (!add(rounded,t->fixed,total)) return false;
    *energy=rounded;return true;
}
