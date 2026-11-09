#include "input.h"
#include <stdio.h>
#include <string.h>
int main(void) {
    char line[256];unsigned line_number=0,accepted=0,rejected=0;
    money_t grand_total=0;
    while (fgets(line,sizeof line,stdin)) {
        ++line_number;
        if (!strchr(line,'\n') && !feof(stdin)) {
            int ch;while ((ch=getchar())!='\n'&&ch!=EOF) {}
            printf("REJECT line %u: line too long\n",line_number);++rejected;continue;
        }
        Reading r;unsigned units;
        if (!parse_reading(line,&r)) {
            printf("REJECT line %u: invalid fields/range/category\n",line_number);++rejected;continue;
        }
        if (!consumption_checked(r.previous,r.current,r.rollover,&units)) {
            printf("REJECT line %u: inconsistent or unconfirmed rollover\n",line_number);++rejected;continue;
        }
        Bill b=bill_for_units(units);
        if (!add_money(&grand_total,b.total_paise)) {
            fprintf(stderr,"Total overflow\n");return 3;
        }
        printf("%s units=%u energy=",r.household,units);print_money(b.energy_paise);
        printf(" fixed=");print_money(b.fixed_paise);printf(" total=");print_money(b.total_paise);putchar('\n');
        ++accepted;
    }
    if (ferror(stdin)) { perror("Input error");return 3; }
    printf("Accepted=%u rejected=%u total=",accepted,rejected);print_money(grand_total);putchar('\n');
    return rejected?2:0;
}
