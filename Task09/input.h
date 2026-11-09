#ifndef CSA02_INPUT_H
#define CSA02_INPUT_H
#include "../common/billing.h"
typedef struct { char household[HOUSEHOLD_ID_SIZE];char category;
    meter_t previous,current;bool rollover; } Reading;
bool parse_reading(const char *line,Reading *record);
#endif
