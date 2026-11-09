#include "smart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
static bool value(const char *s,uint64_t *out) {
    if (!*s) return false;
    for (const char *p=s;*p;p++) if (*p<'0'||*p>'9') return false;
    errno=0;char *end=NULL;unsigned long long n=strtoull(s,&end,10);
    if (errno==ERANGE||*end||n>UINT64_MAX) return false;
    *out=(uint64_t)n;return true;
}
int main(int argc,char **argv) {
    if (argc!=2) { fprintf(stderr,"Usage: smart tariff.cfg < intervals.txt\n");return 2; }
    Tariff tariff;
    if (!load_tariff(argv[1],&tariff)) { fprintf(stderr,"Invalid tariff configuration\n");return 2; }
    Account account={0,0,0};char line[256];uint64_t last_key=0,last_reading=0;
    while (fgets(line,sizeof line,stdin)) {
        char ds[32],ss[32],ps[32],cs[32],extra;uint64_t day,slot,previous,current;
        if ((!strchr(line,'\n')&&!feof(stdin)) ||
            sscanf(line,"%31s %31s %31s %31s %c",ds,ss,ps,cs,&extra)!=4 ||
            !value(ds,&day)||!value(ss,&slot)||!value(ps,&previous)||!value(cs,&current)||
            day<1||day>31||slot>95||current<previous) {
            fprintf(stderr,"Invalid interval; billing aborted\n");return 2;
        }
        uint64_t key=(day-1)*96+slot;
        if (account.intervals&&(key!=last_key+1||previous!=last_reading)) {
            fprintf(stderr,"Missing, repeated or discontinuous interval\n");return 2;
        }
        if (!apply_interval(&account,&tariff,current-previous,(unsigned)slot)) {
            fprintf(stderr,"Interval limit/overflow rejected\n");return 2;
        }
        last_key=key;last_reading=current;
    }
    if (ferror(stdin)||!account.intervals) { fprintf(stderr,"No complete input\n");return 2; }
    uint64_t energy,total;
    if (!finish_bill(&account,&tariff,&energy,&total)) return 3;
    printf("Intervals=%" PRIu64 " used_Wh=%" PRIu64 "\n",account.intervals,account.used_wh);
    printf("Energy=%" PRIu64 " paise; fixed=%" PRIu64 " paise; total=Rs %" PRIu64 ".%02" PRIu64 "\n",
           energy,tariff.fixed,total/100,total%100);
    puts("One supplied billing-period sequence; coverage must be checked before issuing a bill.");
    return 0;
}
