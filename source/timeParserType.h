/*
Copyright 2022 Zorn Link

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef TIME_PARSER_TYPE_H
#define TIME_PARSER_TYPE_H

#include <stdint.h>

#define YEAR_FLAG   (1 << 6)
#define MONTH_FLGA  (1 << 5)
#define DAY_FLAG    (1 << 4)
#define HOUR_FLAG   (1 << 3)
#define MINUTE_FLAG (1 << 2)
#define SECOND_FLAG (1 << 1)

#define TIME_PARSE_FULLY_SUCCESS  4 /*!<解析完全成功。 Parsing completely successful. */
#define TIME_PARSE_ALMOST_SUCCESS 3 /*!< 解析基本成功。 Parsing basic successfully. */
#define TIME_PARSE_PARTLY_SUCCESS 2 /*!< 解析部分成功。 Parsing partially successful. */
#define TIME_PARSE_MAYBE_SUCCESS  1 /*!< 解析可能成功。 Parsing maybe successful. */
#define TIME_PARSE_LITTLE_SUCCESS 0 /*!< 解析基本不成功。 Parsing basic not successful. */

#define TIME_PARSE_STR_ERROR      -1 /*!< Conn't parse the string.无法解析这个字符串*/
#define TIME_PARSE_LANGUAGE_ERROR -2 /*!< The language of string is unknown.无法识别输入的字符串 */

/**
 * @brief 日记解析的结果
 * The result of parsing a journal
 */
typedef struct TimeParserRet_s
{
    int32_t  estimation; /*!< 解析的有效性估计。The estamate of result */
    uint32_t flag;       /*!< 解析的结果标志。The flag of result */
    int32_t  year;       /*!< 年份。The year */
    int32_t  month;      /*!< 月份。The month */
    int32_t  day;        /*!< 日期。The day */
    int32_t  hour;       /*!< 小时。The hour */
    int32_t  minute;     /*!< 分钟。The minute */
    int32_t  second;     /*!< 秒。The second */
} timeParserRet;

#endif