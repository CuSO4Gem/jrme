#ifndef TIME_PARSER_H
#define TIME_PARSER_H

#include <time.h>
#include <stdint.h>

#define YEAR_FLAG   (1<<0)
#define MONTH_FLGA  (1<<1)
#define DAY_FLAG    (1<<2)
#define HOUR_FLAG   (1<<3)
#define MINUTE_FLAG (1<<4)
#define SECOND_FLAG (1<<5)

#define TIME_PARESS_SUCCESS         0
#define TIME_PARESS_PARAMETER_ERROR -1
#define TIME_PARESS_STR_ERROR       -2
#define TIME_PARESS_LANGUAGE_ERROR  -3

typedef struct tm* ptm;

int32_t parseTime(char *str, int32_t len, ptm ptime, uint32_t *flag);

typedef int32_t (*pParseTime)(char *, int32_t, ptm, uint32_t *);

#endif