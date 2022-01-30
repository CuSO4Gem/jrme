#include <dlfcn.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "date.h"
#include "EnTimeParser.h"
#include "timeParserType.h"

using namespace std;
using namespace ec;

/**
 * @brief 
 * 
 * @note 注意：月份是0~11
 * 
 */
struct timeDetail
{
    string sentence;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    uint32_t flag;
};

vector<timeDetail> formateTestCaseSuit = {
    {string("2021-2-1"), 2021, 2, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},
    {string("2021-2-1"), 2021, 2, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},
    {string("1/1/2021"), 2021, 1, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},

    {string("8:33"), 0, 0, 0, 8, 33, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("8:11 am"), 0, 0, 0, 8, 11, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("8:22 pm"), 0, 0, 0, 20, 22, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("18:12"), 0, 0, 0, 18, 12, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("18:23 pm"), 0, 0, 0, 18, 23, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("18:58 am"), 0, 0, 0, 18, 58, 0, HOUR_FLAG|MINUTE_FLAG}
};

vector<timeDetail> describeTestCaseSuit = {
    {string("April 5th"), 0, 4, 5, 9, 0, 0, MONTH_FLGA|DAY_FLAG},
    {string("April 5th"), 0, 4, 5, 9, 0, 0, MONTH_FLGA|DAY_FLAG},

    {string("2018 January 1st"), 2018, 1, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},
    {string("Jan. 2nd 2020"), 2020, 1, 2, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},
    {string("2019 March 3rd"), 2019, 3, 3, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG}
};

bool timePareCmp(const timeParserRet *tRet, const timeDetail *detail)
{
    Date now = Time().toDate();
    
    if ((detail->flag&tRet->flag)!=detail->flag)
    {
        printf("error timePareCmp flag %X VS %X\n", detail->flag, tRet->flag);
        return false;
    }

    if ((detail->flag&YEAR_FLAG) && (tRet->year)!=detail->year)
    {
        printf("error year %d should be %d\n", tRet->year, detail->year);
        return false;
    } else if (!(detail->flag&YEAR_FLAG) && tRet->year!=now.year())
    {
        printf("erro year is %d, it should be now\n", tRet->year);
        return false;
    }

    if ((detail->flag&MONTH_FLGA) && tRet->month!=detail->month)
    {
        printf("error month %d should be %d\n", tRet->month, detail->month);
        return false;
    } else if (!(detail->flag&MONTH_FLGA) && tRet->month!=now.month())
    {
        printf("error month should be now (is %d)", now.month());
        return false;
    }
    if ((detail->flag&DAY_FLAG) && tRet->day!=detail->day)
    {
        printf("error day %d should be %d\n", tRet->day, detail->day);
        return false;
    } else if (!(detail->flag&DAY_FLAG) && tRet->day!=now.day())
    {
        printf("error day should be now");
        return false;
    }

    if ((detail->flag&HOUR_FLAG) && tRet->hour!=detail->hour)
    {
        printf("error hour %d should be %d\n", tRet->hour, detail->hour);
        return false;
    }
    if ((detail->flag&MINUTE_FLAG) && tRet->minute!=detail->minute)
    {
        printf("error minute %d should be %d\n", tRet->minute, detail->minute);
        return false;
    }
    if ((detail->flag&SECOND_FLAG) && tRet->second!=detail->second)
    {
        printf("error second %d should be %d\n", tRet   ->second, detail->second);
        return false;
    }

    return true;
}


TEST(EnTimeParser, sp)
{
    timeDetail detail = {string("2020-2-1"), 2021, 1, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG};

    printf("send to parse:%s\r\n", detail.sentence.c_str());
    EnTimeParser Parser;
    timeParserRet tRet;
    tRet = Parser.parse(detail.sentence);
    // printf("ret = %d time is %d-%d-%d ", ret, timeGot.tm_year+1900, timeGot.tm_mon ,timeGot.tm_mday);
    // printf("%d:%d:%d\n", timeGot.tm_hour, timeGot.tm_min ,timeGot.tm_sec);
}

TEST(EnTimeParser, formate)
{
    EnTimeParser Parser;
    timeParserRet tRet;
    

    for (auto &it:formateTestCaseSuit)
    {
        printf("send to parse:%s\r\n", it.sentence.c_str());
        tRet = Parser.parse(it.sentence);
        ASSERT_GE(tRet.estimation, 1) ;
        ASSERT_TRUE(timePareCmp(&tRet, &it));
    }
}

TEST(EnTimeParser, describe)
{
    EnTimeParser Parser;
    timeParserRet tRet;

    for (auto &it:describeTestCaseSuit)
    {
        printf("send to parse:%s\r\n", it.sentence.c_str());
        tRet = Parser.parse(it.sentence);
        ASSERT_GE(tRet.estimation, 1) ;
        ASSERT_TRUE(timePareCmp(&tRet, &it));
    }
}

int main(int argc, char* argv[]) {
    
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 
    return ret;
}