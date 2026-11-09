#ifndef CSA02_BILLING_H
#define CSA02_BILLING_H
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#define SLAB1_UNITS 100U
#define SLAB2_UNITS 200U
#define RATE1_PAISE 0LL
#ifndef RATE2_PAISE
#define RATE2_PAISE 225LL
#endif
#define RATE3_PAISE 450LL
#define FIXED_CHARGE_PAISE 5000LL
#define METER_MODULUS 65536UL
#define HOUSEHOLD_ID_SIZE 32
_Static_assert(UINT16_MAX == 65535, "Requires a 16-bit unsigned type");
typedef uint16_t meter_t;
typedef long long money_t;
typedef struct {
    unsigned units;
    money_t energy_paise, fixed_paise, total_paise;
} Bill;
bool consumption_checked(meter_t previous, meter_t current,
                         bool rollover_confirmed, unsigned *units);
Bill bill_for_units(unsigned units);
void print_money(money_t paise);
bool add_money(money_t *total, money_t amount);
#endif
