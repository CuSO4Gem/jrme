#ifndef TIME_PARSER_TYPE_H
#define TIME_PARSER_TYPE_H

#include <stdint.h>

#define YEAR_FLAG   (1<<6)
#define MONTH_FLGA  (1<<5)
#define DAY_FLAG    (1<<4)
#define HOUR_FLAG   (1<<3)
#define MINUTE_FLAG (1<<2)
#define SECOND_FLAG (1<<1)

#define TIME_PARSE_FULLY_SUCCESS    4
#define TIME_PARSE_ALMOST_SUCCESS   3
#define TIME_PARSE_PARTLY_SUCCESS   2
#define TIME_PARSE_MAYBE_SUCCESS    1
#define TIME_PARSE_LITTLE_SUCCESS   0

#define TIME_PARSE_STR_ERROR       -1       //Conn't parse the string.无法解析这个字符串
#define TIME_PARSE_LANGUAGE_ERROR  -2       //The language of string is unknown.无法识别输入的字符串

typedef struct TimeParserRet_s
{
    int32_t estimation;
    uint32_t flag;
    int32_t year;
    int32_t month;
    int32_t day;
    int32_t hour;
    int32_t minute;
    int32_t second;
} timeParserRet;

#endif