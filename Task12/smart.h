#ifndef CSA02_SMART_H
#define CSA02_SMART_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    uint64_t slab1_wh,slab2_wh,rate1,rate2,rate3,fixed;
    uint64_t peak_start,peak_end,peak_percent,max_interval_wh;
} Tariff;
typedef struct { uint64_t used_wh,charge_numerator,intervals; } Account;
/* numerator/100000 is the exact charge in paise before final rounding. */
bool load_tariff(const char *path,Tariff *t);
bool apply_interval(Account *a,const Tariff *t,uint64_t delta_wh,unsigned slot);
bool finish_bill(const Account *a,const Tariff *t,uint64_t *energy,uint64_t *total);
#endif
