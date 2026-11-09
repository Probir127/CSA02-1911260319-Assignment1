#include "billing.h"
#include <stdio.h>
bool consumption_checked(meter_t previous, meter_t current,
                         bool rollover_confirmed, unsigned *units) {
    if (!units) return false;
    /* At most one wrap and strictly less than 65536 units per period. */
    if ((current < previous) != rollover_confirmed) return false;
    *units = (uint16_t)((unsigned)current - (unsigned)previous);
    return true;
}
Bill bill_for_units(unsigned units) {
    Bill b = {units, 0, FIXED_CHARGE_PAISE, 0};
    unsigned remaining = units;
    if (remaining > SLAB2_UNITS) {
        b.energy_paise += (money_t)(remaining - SLAB2_UNITS) * RATE3_PAISE;
        remaining = SLAB2_UNITS;
    }
    if (remaining > SLAB1_UNITS) {
        b.energy_paise += (money_t)(remaining - SLAB1_UNITS) * RATE2_PAISE;
        remaining = SLAB1_UNITS;
    }
    b.energy_paise += (money_t)remaining * RATE1_PAISE;
    b.total_paise = b.energy_paise + b.fixed_paise;
#ifdef DEBUG
    fprintf(stderr, "DEBUG units=%u energy=%lld fixed=%lld total=%lld\n",
            units,b.energy_paise,b.fixed_paise,b.total_paise);
#endif
    return b;
}
void print_money(money_t paise) {
    printf("Rs %lld.%02lld", paise/100, paise%100);
}
bool add_money(money_t *total, money_t amount) {
    if (!total || *total < 0 || amount < 0 || *total > LLONG_MAX-amount)
        return false;
    *total += amount;
    return true;
}
