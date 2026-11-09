#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
static bool digits(const char *s) {
    if (!*s) return false;
    for (;*s;s++) if (*s<'0'||*s>'9') return false;
    return true;
}
static bool meter_value(const char *s,meter_t *out) {
    if (!digits(s)) return false;
    errno=0;char *end=NULL;
    unsigned long value=strtoul(s,&end,10);
    if (errno==ERANGE || *end || value>65535UL) return false;
    *out=(meter_t)value;return true;
}
bool parse_reading(const char *line,Reading *record) {
    char id[32],category[8],previous[32],current[32],roll[8],extra;
    if (sscanf(line,"%31s %7s %31s %31s %7s %c",id,category,previous,current,roll,&extra)!=5)
        return false;
    if (!digits(id) || strcmp(category,"D") || (strcmp(roll,"Y") && strcmp(roll,"N")))
        return false;
    if (!meter_value(previous,&record->previous)||!meter_value(current,&record->current))
        return false;
    strcpy(record->household,id);record->category='D';record->rollover=roll[0]=='Y';return true;
}
