#ifndef TIME_PARSER_H
#define TIME_PARSER_H

#include <time.h>
#include <stdint.h>
#include "plugin_common.h"

struct pluginVersion {
    uint16_t mainVersion;
    uint16_t minVersion;
};

NOIT_IT_API const static struct pluginVersion  API_VERSION = {0 ,1};

#define YEAR_FLAG   (1<<0)
#define MONTH_FLGA  (1<<1)
#define DAY_FLAG    (1<<2)
#define HOUR_FLAG   (1<<3)
#define MINUTE_FLAG (1<<4)
#define SECOND_FLAG (1<<5)

#define TIME_PARSE_FULLY_SUCCESS    4
#define TIME_PARSE_ALMOST_SUCCESS   3
#define TIME_PARSE_PARTLY_SUCCESS   2
#define TIME_PARSE_MAYBE_SUCCESS    1
#define TIME_PARSE_LITTLE_SUCCESS   0

#define TIME_PARSE_PARAMETER_ERROR -1       //Input parameter error.输入参数错误
#define TIME_PARSE_STR_ERROR       -2       //Conn't parse the string.无法解析这个字符串
#define TIME_PARSE_LANGUAGE_ERROR  -3       //The language of string is unknown.无法识别输入的字符串

typedef struct tm* ptm;

/**
 * @brief 
 * 
 * @param str 
 * @param len 
 * @param ptime 
 * @param flag return if yesr, month, day, hour, minute, second parse success or not.返回是否成功解析年、月、日、时、分、秒。
 * @return int32_t 
 *         <0: error
 *          0: Find many times, and donm't know which one is true. So return a random on
 *             找的了多个时间，但是不知道哪个是正确的，所以随便给了一个。
 * 
 *          1: Find a time, but also find some word which is no belong to known language
 *             找到了时间，但是多了几个未知语言的字符
 * 
 *          2: Parse time success, but input sentence seemed strange lost some word.(guess some word)
 *             解析成功了，但是输入的句子很奇怪或者似乎丢失了一些单词，所以猜了猜
 * 
 *          3: Parse time success, but there some word seamed useless.
 *             解析成功，但是似乎有一些没用的多余单纯
 * 
 *          4: Fully success
 *             解析完全成功
 */
int32_t parseTime(const char *str, uint32_t len, ptm ptime, uint32_t *flag);

typedef int32_t (*pParseTime)(const char *, int32_t, ptm, uint32_t *);

/**
 * @brief parse time base Regex
 * 
 * @param str intput string
 * @param len string len
 * @param ptime out put time
 * @param flag seach as YEAR_FLAG
 * 
 * @return int32_t TIME_PARSE_
 * 
 */
typedef int32_t (*pParseBaseRegex)(const char *, uint32_t, ptm, uint32_t*);

#endif