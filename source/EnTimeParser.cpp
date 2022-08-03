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
/**
 * @file EnTimeParser.h
 * @brief 将英语的时间描述转换为具体的时间
 * convert english time description to date.
 * @details 大致思路为：将英语关键词转换换成2个字符的“指令”，然后根据指令计算出时间。
 * The principle is that: Convert english keyword to 2-Char instruction set.
 * Then, use the 2-Char instruction set to calculate the date.
 */
#include <algorithm>
#include <list>
#include <regex>
#include <stdio.h>
#include <time.h>

#include "EnTimeParser.h"
#include "date.h"

using ec::Date;
using ec::Time;
using std::list;
using std::min;
using std::regex;
using std::smatch;
using std::swap;
using std::vector;

/**
 * @brief 所有的月份缩写
 * All short-form of english time description.
 */
#define ALL_SHORT_MONTH_REGEX "(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)"

/**
 * @brief 月份缩写与月份号的映射表
 * The map of short-form and month number.
 */

char monthShortName[][4] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};

/**
 * @brief 星期号缩写与星期号的映射表
 * The map of short-form and week day order.
 */

char weekShortName[][4] = {"mon", "the", "wed", "thu", "fir", "sat", "sun"};

/**
 * @brief 表示日期计算指令的结构体
 * The structure of instruction set.
 */
typedef struct tagVmInst
{
    char    cmd[2];     /*!< 指令 */
    bool    haveNumber; /*!< 是否有数字 */
    int32_t number;     /*!< 操作数数字 */
} vmInst;

constexpr int32_t TWOCC(const char *s)
{
    return (unsigned char)*(s) | (unsigned char)*(s + 1) << 8;
}

inline bool isNumChar(const char ch)
{
    return '0' <= ch && ch <= '9';
}

inline bool isLetter(const char ch)
{
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

/**
 * @brief 判断一个字符串是否只包含字母
 * Adjudge whether the string is only letters.
 * @details 如果字符串的结尾为“.”，则其依然会被认为时一个全为字母的字符串。
 * 因为有一些为缩写的单词，结尾就是会有“.”，例如“jan.”。
 * If the end of string is “.”, then it will be regarded as a string.
 * Because some words are abbreviated, the end of string is “.”, such as “jan.”.
 *
 * @param[in] str 字符串
 * @return true 是全字母字符串。is only letters.
 * @return false 不是全字母字符串。is not only letters.
 */
inline bool isLetterOnly(const string &str)
{
    size_t i;
    if (str.length() == 0)
        return false;

    for (i = 0; i < str.length(); i++)
    {
        if (!isLetter(str[i]))
            break;
    }
    if (i == str.length())
        return true;
    // some word may have '.', such as, Feb.
    else if (i == str.length() - 1 && str[i] == '.')
        return true;
    else
        return false;
}

/**
 * @brief 计算最低非0位的位置
 * Return the position of the lowest non-zero bit.
 *
 * @param[in] b 字节
 * @return uint32_t 最低非0位的位置。
 * The position of the lowest non-zero bit.
 */
inline uint32_t lastBitOrder(uint32_t b)
{
    int i;
    if (b == 0)
        return 0;
    else
    {
        i = 0;
        while ((b & 0x1) == 0)
        {
            b = b >> 1;
            i++;
        }
    }
    return i;
}

/**
 * @brief 找到从pos开始的下一个单词开头（不计算头的空格）
 * Find the next word beginning from pos, Not counting the head space.
 * @param[in] str 字符串。string
 * @param[in] pos 开始位置。Beginning position.
 * @return size_t 下一个单词的开头
 */
size_t nextWord(const string &str, size_t pos)
{
    if (pos >= str.length())
    {
        return string::npos;
    }
    /* 跳过前面的空格。Skip the front spaces. */
    while (str[pos] == ' ' && pos < str.length())
    {
        pos++;
    }
    if (pos >= str.length())
        return string::npos;

    /* 跳过第一个单词。Skip the first word */
    while (pos < str.length())
    {
        if (str[pos] == ' ')
            break;
        pos++;
    }
    if (pos >= str.length())
        return string::npos;

    /* 跳过后面的空格。Skip next spaces. */
    while (str[pos] == ' ' && pos < str.length())
    {
        pos++;
    }
    if (pos >= str.length())
        return string::npos;

    if (pos >= str.length())
        return string::npos;
    else
        return pos;
}

/**
 * @brief 获取字符串的第一个单词
 * Get the first word of string.
 *
 * @param[in] str 字符串。string
 * @return string 第一个单词。First word.
 */
string getWord(const string &str)
{
    size_t pos = 0;
    /*skip space in front*/
    while (str[pos] == ' ' && pos < str.length())
    {
        pos++;
    }
    if (pos >= str.length())
        return string("");
    size_t begin = pos;

    while (str[pos] != ' ' && pos < str.length())
    {
        pos++;
    }
    size_t end = pos;
    return str.substr(begin, end - begin);
}

/**
 * @brief 擦除字符串的第一个单词
 * Erase the first word of string.
 *
 * @param str 字符串。string
 */
void eraseWrod(string &str)
{
    size_t pos = nextWord(str, 0);
    if (pos == string::npos)
        str.clear();
    else if (pos >= 0)
        str.erase(0, pos);
    else
        return;
}

/**
 * @brief 将所有的日期计算指令打印出来。
 * Print all the date calculation instructions.
 *
 * @param[in] instVector 日期计算指令。Date calculation instructions.
 */
void printVmInst(vector<vmInst> &instVector)
{
    for (auto &it : instVector)
    {
        printf("|%c%c", it.cmd[0], it.cmd[1]);
        if (it.haveNumber)
            printf("%d", it.number);
    }
    printf("\n");
}

/**
 * @brief 将日期描述语句解析为具体时间的类。之所以要这个类，是因为以后可能会开发插件来支持各种语言
 * Parse the date description sentence to a class that can represent the specific time.
 * The Reason why I need this class is because. I will develop a plugin to support various languages.
 *
 */
class Parser
{
  public:
    Parser(const string &intStr);
    timeParserRet getTime();

  private:
    int32_t        mEstimation;
    string         mInStr;
    string         mRemainStr;
    vector<vmInst> mVmInst;
    Date           mParseResult;
    uint32_t       mParseFlag;

    void   instSrot(vector<vmInst> &instVector);
    Date   parseVM(vector<vmInst> &vmInsts, uint32_t *resultFlag, int32_t *estimation);
    void   prepare();
    void   parseAmPm(string &words);
    size_t parseSampleUnit(string &words, vector<vmInst> &instVector);
    void   parseFormat(string &words, vector<vmInst> &instVector);
    void   parsePhrase(string &words, vector<vmInst> &instVector);
    bool   setDateForce(Date &dst, uint32_t *dstFlag, Date &src, const uint32_t *srcFlag);
    bool   setDateWeek(Date &dst, uint32_t *dstFlag, Date &src, const uint32_t *srcFlag);
    void   removeMultipleSpaces(string &str);
};

/**
 * @brief 构造函数
 * Constructor
 * @details 会将字符串全部转换为小写
 * String will be converted to lower case.
 *
 * @param[in] intStr 需要解析的日期描述语句。Date description sentence.
 */
Parser::Parser(const string &intStr)
{
    mEstimation = 5;
    mInStr      = intStr;
    transform(mInStr.begin(), mInStr.end(), mInStr.begin(), ::tolower);
    mRemainStr = "";
}

/**
 * @brief 获取具体时间。
 * Get the specific time.
 *
 * @return timeParserRet
 */
timeParserRet Parser::getTime()
{
    timeParserRet tRet;
    prepare();

    list<vector<vmInst>> instHolder;

    parseAmPm(mRemainStr);
    parseFormat(mRemainStr, mVmInst);
    parsePhrase(mRemainStr, mVmInst);

    string word;
    bool   wordLastFlag = false;
    /**
     * 逐个单词进行解析，将它们转换为日记计算指令。
     * Parse each word and convert it to date calculation instructions.
     */
    while ((word = getWord(mRemainStr)).length() != 0)
    {
        smatch regexResult;
        bool   haveNumber    = false;
        bool   haveLetter    = false;
        bool   haveOtherChar = false;

        for (size_t i = 0; i < word.length(); i++)
        {
            if (isNumChar(word[i]))
                haveNumber = true;
            else if (isLetter(word[i]))
                haveLetter = true;
            // some word end with '.'
            else if (i == word.length() - 1 && word[i] == '.')
                ;
            else
            {
                haveOtherChar = true;
                break;
            }
        }

        if (word == string("last"))
        {
            wordLastFlag = true;
            eraseWrod(mRemainStr);
            continue;
        }
        if (word == string("ago") && !instHolder.empty())
        {
            vector<vmInst> &lastInst = *(prev(instHolder.end()));
            size_t          i        = 0;
            for (auto &it : lastInst)
            {
                if (it.haveNumber)
                {
                    it.number = -abs(it.number);
                }
            }
        }
        /* 4 number, maybe year*/
        if (!haveLetter && haveNumber && !haveOtherChar && word.length() == 4)
        {
            /* can not "last yyyy" in sentence*/
            if (wordLastFlag)
            {
                wordLastFlag = false;
                mEstimation  = min(mEstimation, TIME_PARSE_PARTLY_SUCCESS);
            }
            vector<vmInst> instVector;
            instVector.push_back(vmInst{'y', 'y', true, std::atoi(word.c_str())});
            instHolder.push_back(instVector);
            eraseWrod(mRemainStr);
            continue;
        }
        if (word == string("today"))
        {
            /* can not "last today" in sentence*/
            if (wordLastFlag)
            {
                wordLastFlag = false;
                mEstimation  = min(mEstimation, TIME_PARSE_PARTLY_SUCCESS);
            }
            vector<vmInst> instVector;
            instVector.push_back(vmInst{'d', 'd', false, 0});
            instHolder.push_back(instVector);
            eraseWrod(mRemainStr);
            continue;
        }
        if (word == string("yesterday"))
        {
            /* can not "last yesterday" in sentence*/
            if (wordLastFlag)
            {
                wordLastFlag = false;
                mEstimation  = min(mEstimation, TIME_PARSE_PARTLY_SUCCESS);
            }
            vector<vmInst> instVector;
            instVector.push_back(vmInst{'d', 'd', true, -1});
            instHolder.push_back(instVector);
            eraseWrod(mRemainStr);
            continue;
        }
        /**
         * 包含数字且以st，nd，rd，th结尾的单词，解析为日期。
         * Word contains number and end with st, nd, rd, th, parse it as day of month.
         */
        if (regex_search(word, regexResult, regex("(\\d{1,2})(st|nd|rd|th)")))
        {
            vector<vmInst> instVector;
            instVector.push_back(vmInst{'m', 'd', true, std::atoi(word.c_str())});
            instHolder.push_back(instVector);
            eraseWrod(mRemainStr);
            continue;
        }
        if (!haveOtherChar)
        {
            vector<vmInst> instVector;
            size_t         readSize = parseSampleUnit(mRemainStr, instVector);
            if (readSize > 0 && !instVector.empty())
            {
                mRemainStr.erase(0, readSize);
                if (wordLastFlag)
                {
                    wordLastFlag = false;
                    for (auto &it : instVector)
                    {
                        /**
                         * 在单词last后面时，haveNumber==fale也是可以的，
                         * 如:last year。这个时候，parseSampleUnit接续出来
                         * number应该是1
                         */
                        it.haveNumber = true;
                        if (it.number > 0)
                            it.number = -abs(it.number);
                    }
                }

                instHolder.push_back(instVector);
                continue;
            }
        }
        /*to do estimate*/
        eraseWrod(mRemainStr);
    }

    for (auto &holderIt : instHolder)
    {
        for (auto &it : holderIt)
        {
            mVmInst.push_back(it);
        }
    }
    instSrot(mVmInst);

    mParseResult    = parseVM(mVmInst, &mParseFlag, &mEstimation);
    tRet.estimation = mEstimation;
    tRet.flag       = mParseFlag;
    tRet.year       = mParseResult.year();
    tRet.month      = mParseResult.month();
    tRet.day        = mParseResult.day();
    tRet.hour       = mParseResult.hour();
    tRet.minute     = mParseResult.minute();
    tRet.second     = mParseResult.second();
    return tRet;
}

/**
 * @brief 依次处理日期计算指令，计算出日期。
 * Process each date calculation instruction, calculate the date.
 *
 * @param[in] vmInsts 指令。instructions
 * @param[in] resultFlag 有效的日期标志。valid date flag
 * @param[in] estimation 对结果的可信度估计。Reliable of the result.
 * @return Date
 */
Date Parser::parseVM(vector<vmInst> &vmInsts, uint32_t *resultFlag, int32_t *estimation)
{
    Time     ecTime;
    Date     nowDate    = ecTime.toDate();
    Date     resultDate = ecTime.toDate();
    uint32_t flagOut = 0, flagWeekOut = 0;
    bool     pm = false;

    for (auto &it : vmInsts)
    {
        Date     tmpDate = resultDate;
        uint32_t flag = 0, flagWeek = 0;
        int      weekDay;
        switch (TWOCC(it.cmd))
        {
        case TWOCC("am"):
            pm = false;
            break;

        case TWOCC("pm"):
            pm = true;
            break;

        /**
         * 一年中的某一天
         * Day of year in a year
         */
        case TWOCC("dd"):
            tmpDate.set(nowDate.year(), nowDate.month(), nowDate.day(), 9, 0, 0);
            if (it.haveNumber && it.number <= 0)
                tmpDate = tmpDate.toTime().addDay(it.number);
            else if (it.haveNumber && it.number > 0)
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            flag = YEAR_FLAG | MONTH_FLGA | DAY_FLAG;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;
            break;

        /**
         * 具体的某一年
         * Specific year
         */
        case TWOCC("yy"):
            tmpDate = resultDate;
            if (!it.haveNumber)
                tmpDate.setYear(nowDate.year());
            else if (0 == it.number)
                tmpDate.setYear(nowDate.year());
            else if (it.number < 0 && tmpDate.year() + it.number > 1970)
                tmpDate.addYear(it.number);
            else if (it.number > 1970)
                tmpDate.setYear(it.number);
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            flag = YEAR_FLAG;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;
            break;

        /**
         * 一年中具体的某一月
         * Specific month in a year
         */
        case TWOCC("mo"):
            tmpDate = resultDate;
            flag    = MONTH_FLGA;
            if (!it.haveNumber)
                tmpDate.setMonth(nowDate.month());
            else if (it.haveNumber == 0)
                tmpDate.setMonth(nowDate.month());
            else if (1 <= it.number && it.number <= 12)
                tmpDate.setMonth(it.number);
            else if (-12 <= it.number && it.number <= -1)
            {
                /**
                 * 如果数字小于0则说明是上一年的月份，
                 * If the number is less than 0, it means the previous year's month
                 */
                tmpDate.setMonth(abs(it.number));
                tmpDate.setYear(resultDate.year() - 1);
                flag |= YEAR_FLAG;
            }
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;

            break;

        /**
         * 关于当前时间的月份偏移
         * Month offset of current time
         */
        case TWOCC("mn"):

            if (!it.haveNumber)
                ;
            else if (it.number = 0)
                ;
            else if (it.number < 0)
                tmpDate.addMonth(it.number);
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            flag = YEAR_FLAG | MONTH_FLGA;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;
            break;

        /**
         * 一个月中的某一天
         * Day of month in a month
         */
        case TWOCC("md"):
            tmpDate = resultDate;

            if (it.haveNumber && 1 <= it.number && it.number <= 31)
            {
                tmpDate.setDay(it.number);
                if (tmpDate.month() != resultDate.month())
                {
                    *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                    break;
                }
            }
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            flag = DAY_FLAG;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;

            break;

        /**
         * 一个月中的某一周
         * Week in a month
         */
        case TWOCC("ww"):
            tmpDate = resultDate;
            weekDay = resultDate.week();
            if (!it.haveNumber)
            {
                if (weekDay != 1)
                {
                    tmpDate = tmpDate.toTime().addDay(-(weekDay - 1));
                }
            }
            else if (it.number == 0)
            {
                if (weekDay != 1)
                {
                    tmpDate = tmpDate.toTime().addDay(-(weekDay - 1));
                }
            }
            else if (it.number < 0)
            {
                tmpDate = tmpDate.toTime().addDay(-(weekDay - 1 + 7 * it.number));
            }
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            flagWeek = YEAR_FLAG | MONTH_FLGA | DAY_FLAG;
            setDateWeek(resultDate, &flagOut, tmpDate, &flagWeek);
            flagWeekOut |= flagWeek;
            break;

        /**
         * 一个星期中的某一个
         * Day in a week
         */
        case TWOCC("wd"):
            tmpDate = resultDate;
            if (!it.haveNumber)
                break;
            else if (it.number > 0)
            {
                int dayOffast = resultDate.week() - it.number;
                resultDate    = resultDate.toTime().addDay(dayOffast).toDate();
                if (nowDate < resultDate)
                {
                    resultDate.toTime().addDay(-7).toDate();
                }
            }
            else if (it.number < 0)
            {
                int dayOffast = resultDate.week() - abs(it.number);
                resultDate    = resultDate.toTime().addDay(dayOffast - 7).toDate();
            }
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }
            flagOut |= YEAR_FLAG | MONTH_FLGA | DAY_FLAG;
            break;

        /**
         * 一天中的某一小时
         * Hour in a day
         */
        case TWOCC("hh"):
            if (!it.haveNumber)
                break;
            else if (it.number >= 0)
            {
                if (pm && it.number <= 12)
                    tmpDate.setHour((it.number + 12) % 24);
                else
                    tmpDate.setHour(it.number % 24);
            }
            /**
             * 如果数字小于0则说明是n个小时前。
             * If the number is less than 0, it means n hours ago.
             */
            else if (it.number < 0)
            {
                tmpDate.toTime().addHour(it.number);
            }
            else
                break;

            flag = HOUR_FLAG;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;
            break;

        /**
         * 具体的某一分钟
         * Specific minute
         */
        case TWOCC("mi"):
            if (!it.haveNumber)
                break;
            else if (it.number >= 0)
            {
                tmpDate.setMinute(it.number);
            }
            else if (it.number < 0)
            {
                tmpDate.toTime().addMinute(it.number);
            }
            else
                break;

            flag = MINUTE_FLAG;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;
            break;

        /**
         * 具体的某一秒
         * Specific second
         */
        case TWOCC("ss"):
            if (!it.haveNumber)
                break;
            else if (it.number >= 0)
            {
                tmpDate.setSecond(it.number);
            }
            else if (it.number < 0)
            {
                tmpDate.toTime().setSeconds(it.number);
            }
            else
                break;

            flag = SECOND_FLAG;
            if (setDateForce(resultDate, &flagOut, tmpDate, &flag))
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
            }
            flagOut |= flag;
            break;

        /**
         * 不是一个准确的小时，只是一个粗略的时间。比如morning关键词。
         * Not a precise hour, only a rough time. For example, "morning" keyword.
         */
        case TWOCC("hn"):
            if (((flagOut | flagWeek) & (HOUR_FLAG | MINUTE_FLAG | SECOND_FLAG)) != 0)
                break;
            else if (it.haveNumber && it.number <= 24)
            {
                resultDate.setHour(it.number % 24);
                resultDate.setMinute(0);
                resultDate.setSecond(0);
                flagWeek |= HOUR_FLAG | MINUTE_FLAG | SECOND_FLAG;
            }
            else
            {
                *estimation = min(TIME_PARSE_PARTLY_SUCCESS, *estimation);
                break;
            }

            break;

        default:
            break;
        }
    }

    if (resultDate.year() < 1970)
    {
        *estimation = min(TIME_PARSE_LITTLE_SUCCESS, *estimation);
    }

    *resultFlag = flagOut | flagWeekOut;

    /**
     * 如果日期不是今天了，且没有设置 时分秒，那么就设置在早上9点
     * If the date is not today and not set the time, set it at 9:00 am.
     */
    bool dayChange = true;
    if (nowDate.year() == resultDate.year() && nowDate.month() == resultDate.month() &&
        nowDate.day() == resultDate.day())
    {
        dayChange = false;
    }
    if (dayChange && (*resultFlag & (HOUR_FLAG | MINUTE_FLAG | SECOND_FLAG)) == 0)
    {
        resultDate.setHour(9);
        resultDate.setMinute(0);
        resultDate.setSecond(0);
    }

    return resultDate;
}

/**
 * @brief 将日记指令进行排序。需要优先处理的指令排在前面。
 * Order the command list. The command that need to be processed first will be placed in front of the list.
 * @details 指令顺序为am，pm，dd，yy，mo，mn，md。
 * Order of the command is am, pm, dd, yy, mo, mn, md.
 *
 * @param instVector
 */
void Parser::instSrot(vector<vmInst> &instVector)
{
    char    orderTable[][2] = {{'a', 'm'}, {'p', 'm'}, {'d', 'd'}, {'y', 'y'}, {'m', 'o'}, {'m', 'n'}, {'m', 'd'}};
    int32_t notOrderNumb    = 0;
    int32_t scanNumb;
    int32_t orderNumb;
    for (orderNumb = 0; orderNumb < sizeof(orderTable); orderNumb++)
    {
        for (scanNumb = notOrderNumb; scanNumb < instVector.size(); scanNumb++)
        {
            if (instVector[scanNumb].cmd[0] == orderTable[orderNumb][0] &&
                instVector[scanNumb].cmd[1] == orderTable[orderNumb][1])
            {
                swap(instVector[notOrderNumb], instVector[scanNumb]);
                notOrderNumb++;
            }
        }
    }
}

/**
 * @brief 字符预处理。Prepare
 * @details 将in,on,at统一转换成in，并删除多余的空格。
 * Convert in,on,at to in, and delete extra spaces.
 */
void Parser::prepare()
{
    uint32_t spaceAccount = 0;
    char    *strBuffer    = (char *)malloc(mInStr.length() + 1);
    size_t   j            = 0;
    for (size_t i = 0; i < mInStr.length(); i++)
    {
        char ch;
        if (mInStr[i] == ' ' || mInStr[i] == '\t')
            spaceAccount++;
        else
            spaceAccount = 0;

        if (spaceAccount > 1 || mInStr[i] == '|')
            continue;
        else if (mInStr[i] == '\t')
            ch = ' ';
        else
            ch = mInStr[i];

        strBuffer[j] = ch;
        j++;
    }
    strBuffer[j] = '\0';
    mRemainStr   = string(strBuffer);
    free(strBuffer);

    while (mRemainStr.find("on ") != string::npos)
    {
        mRemainStr.replace(mRemainStr.find("on "), 2, "in");
    }
    while (mRemainStr.find("at ") != string::npos)
    {
        mRemainStr.replace(mRemainStr.find("at "), 2, "in");
    }
}

/**
 * @brief 将AM和AM关键字解析成日期计算指令并放入mVmInst。
 * Parse AM and AM keywords to date calculation instructions and put them in mVmInst.
 *
 * @param[in] words 要解析的字符串。The string to be parsed.
 */
void Parser::parseAmPm(string &words)
{
    if (words.length() == 0)
        return;

    int pos = 0;
    while ((pos = words.find("am", pos)) != string::npos)
    {
        //一个合格的am，前后应该只有空格或者数字
        if ((pos == 0 || isNumChar(words[pos - 1]) || words[pos - 1] == ' ') &&
            (words[pos + 2] == '\0' || isNumChar(words[pos + 2]) || words[pos + 2] == ' '))
        {
            vmInst inst = {{'a', 'm'}, false, 0};
            mVmInst.push_back(inst);
            words.replace(pos, 2, " ");
        }
        else
            pos++;
    }

    pos = 0;
    while ((pos = words.find("pm", pos)) != string::npos)
    {
        //一个合格的pm，前后应该只有空格或者数字
        if ((pos == 0 || isNumChar(words[pos - 1]) || words[pos - 1] == ' ') &&
            (words[pos + 2] == '\0' || isNumChar(words[pos + 2]) || words[pos + 2] == ' '))
        {
            vmInst inst = {{'p', 'm'}, false, 0};
            mVmInst.push_back(inst);
            words.replace(pos, 2, " ");
        }
        else
            pos++;
    }
    removeMultipleSpaces(words);
}

/**
 * @brief 尝试解析简单带单位时间的指令。Parse simple time with unit instructions.
 *
 * @param[in] words 要解析的字符串。The string to be parsed.
 * @param[out] instVector 返回的命令。Returned command.
 * @return size_t 读取长度。Read length.
 */
size_t Parser::parseSampleUnit(string &words, vector<vmInst> &instVector)
{
    if (words.length() == 0)
        return -1;

    /*一个合格的单位应该只有字母*/
    size_t unitBegin = 0;
    string unit      = getWord(words);
    if (unit.length() == 0)
        return -1;

    /**
     * 如果第一个单词不是纯字母，考虑看看第二个，因为前一个可能是数字
     * If the first word is not pure letter, consider the second word, because the first may be a number.
     */
    if (!isLetterOnly(unit))
    {
        unitBegin = nextWord(words, unitBegin);
        if (unitBegin == string::npos)
            return -1;
        unit = getWord(words.substr(unitBegin, words.length() - unitBegin));
        if (unit.length() == 0)
            return -1;
    }
    if (!isLetterOnly(unit))
    {
        return -1;
    }

    size_t unitEnd = unitBegin;
    while (unitEnd < words.length())
    {
        if (isLetter(words[unitEnd]))
            unitEnd++;
        else
            break;
    }

    size_t  numberBegin = 0;
    int32_t number      = -1;
    while (numberBegin < words.length() && numberBegin < unitBegin)
    {
        if (isNumChar(words[numberBegin]))
        {
            number = atoi(words.c_str() + numberBegin);
            break;
        }

        numberBegin++;
    }

    if (unit == string("year") || unit == string("years"))
    {
        instVector.push_back(vmInst{{'y', 'y'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }

    if (unit == string("month") || unit == string("month"))
    {
        instVector.push_back(vmInst{{'m', 'n'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }
    for (int32_t i = 0; i < 12; i++)
    {
        if (unit.find(monthShortName[i]) != string::npos)
        {
            instVector.push_back(vmInst{{'m', 'o'}, true, i + 1});
            if (number > 0)
            {
                instVector.push_back(vmInst{{'m', 'd'}, true, number});
            }
            return unitEnd;
        }
    }

    if (unit == string("weekend"))
    {
        instVector.push_back(vmInst{{'w', 'n'}, false, 1});
        return unitEnd;
    }

    if (unit == string("week"))
    {
        instVector.push_back(vmInst{{'w', 'w'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }
    for (int32_t i = 0; i < 7; i++)
    {
        if (unit.find(weekShortName[i]) != string::npos)
        {
            instVector.push_back(vmInst{{'w', 'd'}, true, i + 1});
            return unitEnd;
        }
    }
    if (unit == string("day") || unit == string("days"))
    {
        instVector.push_back(vmInst{{'d', 'd'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }
    if (unit == string("morning"))
    {
        instVector.push_back(vmInst{{'h', 'n'}, true, 9});
        return unitEnd;
    }
    if (unit == string("noon"))
    {
        instVector.push_back(vmInst{{'h', 'n'}, true, 12});
        return unitEnd;
    }
    if (unit == string("afternoon"))
    {
        instVector.push_back(vmInst{{'h', 'n'}, true, 14});
        return unitEnd;
    }
    if (unit == string("night"))
    {
        instVector.push_back(vmInst{{'h', 'n'}, true, 21});
        return unitEnd;
    }
    if (unit == string("midnight"))
    {
        instVector.push_back(vmInst{{'h', 'n'}, true, 24});
        return unitEnd;
    }
    if (unit == string("hour") || unit == string("hours"))
    {
        instVector.push_back(vmInst{{'h', 'h'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }
    if (unit == string("minute") || unit == string("minutes"))
    {
        instVector.push_back(vmInst{{'m', 'i'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }

    if (unit == string("second") || unit == string("seconds"))
    {
        instVector.push_back(vmInst{{'s', 'i'}, number > -1, number > -1 ? number : 1});
        return unitEnd;
    }

    return unitEnd;
}

/**
 * @brief 解析常见的格式化时间表达式，例如YYYY-MM-DD HH:MM:SS
 * Parse common formatted time expressions, Such as YYYY-MM-DD HH:MM:SS
 *
 * @param[in] words 要解析的字符串。String to be parsed.
 * @param[out] instVector 输出的命令。Output commands.
 */
void Parser::parseFormat(string &words, vector<vmInst> &instVector)
{
    if (words.length() == 0)
        return;
    smatch regexResult;

    // YYY-MM-DD
    regex pattern = regex("(\\d{4})[^:]{1,1}(\\d{1,2})[^:]{1,1}(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) && atoi(regexResult.str(1).c_str()) >= 1970 &&
        atoi(regexResult.str(2).c_str()) <= 12 && atoi(regexResult.str(3).c_str()) <= 31)
    {
        instVector.push_back(vmInst{{'y', 'y'}, true, atoi(regexResult.str(1).c_str())});
        instVector.push_back(vmInst{{'m', 'o'}, true, atoi(regexResult.str(2).c_str())});
        instVector.push_back(vmInst{{'m', 'd'}, true, atoi(regexResult.str(3).c_str())});
        words = regex_replace(words, pattern, "");
    }

    // DD-MM-YYYY
    pattern = regex("(\\d{1,2})[^:]{1}(\\d{1,2})[^:]{1}(\\d{4})");
    if (regex_search(words, regexResult, pattern) && atoi(regexResult.str(1).c_str()) <= 31 &&
        atoi(regexResult.str(2).c_str()) <= 12 && atoi(regexResult.str(3).c_str()) >= 1970)
    {
        instVector.push_back(vmInst{{'m', 'd'}, true, atoi(regexResult.str(1).c_str())});
        instVector.push_back(vmInst{{'m', 'o'}, true, atoi(regexResult.str(2).c_str())});
        instVector.push_back(vmInst{{'y', 'y'}, true, atoi(regexResult.str(3).c_str())});
        words = regex_replace(words, pattern, "");
    }

    // YY-MM
    pattern = regex("(\\d{4})[^:]{1}(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) && atoi(regexResult.str(2).c_str()) <= 31)
    {
        instVector.push_back(vmInst{{'y', 'y'}, true, atoi(regexResult.str(1).c_str())});
        instVector.push_back(vmInst{{'m', 'o'}, true, atoi(regexResult.str(2).c_str())});
        words = regex_replace(words, pattern, "");
    }

    // hh:mm:ss
    pattern = regex("(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) && atoi(regexResult.str(1).c_str()) <= 24 &&
        atoi(regexResult.str(2).c_str()) <= 60 && atoi(regexResult.str(3).c_str()) <= 60)
    {
        instVector.push_back(vmInst{{'h', 'h'}, true, atoi(regexResult.str(1).c_str())});
        instVector.push_back(vmInst{{'m', 'i'}, true, atoi(regexResult.str(2).c_str())});
        instVector.push_back(vmInst{{'s', 's'}, true, atoi(regexResult.str(3).c_str())});
        words = regex_replace(words, pattern, "");
    }

    // hh:mm
    pattern = regex("(\\d{1,2}):(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) && atoi(regexResult.str(1).c_str()) <= 24 &&
        atoi(regexResult.str(2).c_str()) <= 60)
    {
        instVector.push_back(vmInst{{'h', 'h'}, true, atoi(regexResult.str(1).c_str())});
        instVector.push_back(vmInst{{'m', 'i'}, true, atoi(regexResult.str(2).c_str())});
        words = regex_replace(words, pattern, "");
    }
    removeMultipleSpaces(words);
}

/**
 * @brief 在words中找到常见词组，解析成命令并将解析过的词组擦除。
 *
 * @param words
 * @param instVector 输出的命令
 */
void Parser::parsePhrase(string &words, vector<vmInst> &instVector)
{
    string strFind;
    strFind = string("the year before last year");
    if (words.find(strFind) != string::npos)
    {
        words.replace(words.find(strFind), strFind.length(), "");
        instVector.push_back(vmInst{{'y', 'y'}, true, -2});
    }

    strFind = string("the month before last month");
    if (words.find(strFind) != string::npos)
    {
        words.replace(words.find(strFind), strFind.length(), "");
        instVector.push_back(vmInst{{'m', 'o'}, true, -2});
    }

    strFind = string("the day before yesterday");
    if (words.find(strFind) != string::npos)
    {
        words.replace(words.find(strFind), strFind.length(), "");
        instVector.push_back(vmInst{{'d', 'd'}, true, -2});
    }

    removeMultipleSpaces(words);
    return;
}

/**
 * @brief 删除字符串中连续的多个空格
 * Remove multiple spaces in a string
 * @param[in,out] str 字符串。String to be processed.
 */
void Parser::removeMultipleSpaces(string &str)
{
    uint32_t i        = 0;
    uint32_t gotSpace = 0;
    while (i < str.length())
    {
        if (str[i] == ' ')
            gotSpace++;
        else
            gotSpace = 0;

        if (gotSpace > 1)
        {
            str.erase(i, 1);
        }
        else
        {
            i++;
        }
    }
}

/**
 * @brief 设置时间并检测冲突，无论有没有冲突都会设置时间，但是返回值不同
 * Set time and check conflict. Whatever there is conflict or not, it will set time, but return value is different.
 *
 * @param[out] dst 目标时间。Destination time.
 * @param[in] dstFlag 目标时间有效的标记。Destination time flag.
 * @param[in] src 源时间。Source time.
 * @param[in] srcFlag 希望设置目标时间的部分。Source time flag.
 * @return true 设置时间时发现有冲突。True if there is conflict when setting time.
 * @return false 设置时间时没发现冲突。False if there is no conflict when setting time.
 */
bool Parser::setDateForce(Date &dst, uint32_t *dstFlag, Date &src, const uint32_t *srcFlag)
{
    bool conflict = false;

    if (lastBitOrder(*srcFlag) <= lastBitOrder(YEAR_FLAG))
    {
        if ((*dstFlag & YEAR_FLAG) && dst.year() != src.year())
            conflict = true;
        dst.setYear(src.year());
    }
    if (lastBitOrder(*srcFlag) <= lastBitOrder(MONTH_FLGA))
    {
        if ((*dstFlag & MONTH_FLGA) && dst.month() != src.month())
            conflict = true;
        dst.setMonth(src.month());
    }
    if (lastBitOrder(*srcFlag) <= lastBitOrder(DAY_FLAG))
    {
        if ((*dstFlag & DAY_FLAG) && dst.day() != src.day())
            conflict = true;
        dst.setDay(src.day());
    }
    if (lastBitOrder(*srcFlag) <= lastBitOrder(HOUR_FLAG))
    {
        if ((*dstFlag & HOUR_FLAG) && dst.hour() != src.hour())
            conflict = true;
        dst.setHour(src.hour());
    }
    if (lastBitOrder(*srcFlag) <= lastBitOrder(MINUTE_FLAG))
    {
        if ((*dstFlag & MINUTE_FLAG) && dst.minute() != src.minute())
            conflict = true;
        dst.setMinute(src.minute());
    }
    if (lastBitOrder(*srcFlag) <= lastBitOrder(SECOND_FLAG))
    {
        if ((*dstFlag & SECOND_FLAG) && dst.second() != src.second())
            conflict = true;
        dst.setSecond(src.second());
    }
    return conflict;
}

/**
 * @brief 设一个月中的某一周并检测冲突，无论有没有冲突都会设置时间，但是返回值不同
 * Set week in month. Whatever there is conflict or not, it will set time, but return value is different.
 *
 * @param[out] dst 目标时间。Destination time.
 * @param[in] dstFlag 目标时间有效的标记。Destination time flag.
 * @param[in] src 源时间。Source time.
 * @param[in] srcFlag 希望设置目标时间的部分。Source time flag.
 * @return true 设置时间时发现有冲突。True if there is conflict when setting time.
 * @return false 设置时间时没发现冲突。False if there is no conflict when setting time.
 */
bool Parser::setDateWeek(Date &dst, uint32_t *dstFlag, Date &src, const uint32_t *srcFlag)
{
    bool conflict = false;
    if (lastBitOrder(*srcFlag) >= YEAR_FLAG)
    {
        if ((*dstFlag & YEAR_FLAG) && dst.year() != src.year())
            conflict = true;
        else
            dst.setYear(src.year());
    }
    if (lastBitOrder(*srcFlag) >= MONTH_FLGA)
    {
        if ((*dstFlag & MONTH_FLGA) && dst.month() != src.month())
            conflict = true;
        else
            dst.setMonth(src.month());
    }
    if (lastBitOrder(*srcFlag) >= DAY_FLAG)
    {
        if ((*dstFlag & DAY_FLAG) && dst.day() != src.day())
            conflict = true;
        else
            dst.setDay(src.day());
    }
    if (lastBitOrder(*srcFlag) >= HOUR_FLAG)
    {
        if ((*dstFlag & HOUR_FLAG) && dst.hour() != src.hour())
            conflict = true;
        else
            dst.setHour(src.hour());
    }
    if (lastBitOrder(*srcFlag) >= MINUTE_FLAG)
    {
        if ((*dstFlag & MINUTE_FLAG) && dst.minute() != src.minute())
            conflict = true;
        else
            dst.setMinute(src.minute());
    }
    if (lastBitOrder(*srcFlag) >= SECOND_FLAG)
    {
        if ((*dstFlag & SECOND_FLAG) && dst.second() != src.second())
            conflict = true;
        else
            dst.setSecond(src.second());
    }
    return conflict;
}

/**
 * @brief 将英语的时间描述转换为具体的时间
 * convert english time description to date.
 * @param description 英语的时间描述。English time description.
 * @return timeParserRet 时间解析结果。Time parser result.
 */
timeParserRet EnTimeParser::parse(string description)
{
    Parser timeParser(description);
    return timeParser.getTime();
}