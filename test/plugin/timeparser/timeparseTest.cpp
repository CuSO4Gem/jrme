#include <dlfcn.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "timeparser_plugin.h"

using namespace std;

#define PLAG_NAME "timeparser_en"
#define PLAG_SUBFIX "so"

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

/**
 * @brief 
 * 
 * @note 注意：月份是0~11
 * 
 */
vector<timeDetail> formateTestCase = {
    {string("2021-2-1"), 2021, 1, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},
    {string("2021-2-1"), 2021, 1, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},
    {string("1/1/2021"), 2021, 0, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG},

    {string("8:33"), 0, 0, 0, 8, 33, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("8:11 am"), 0, 0, 0, 8, 11, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("8:22 pm"), 0, 0, 0, 20, 22, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("18:12"), 0, 0, 0, 18, 12, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("18:23 pm"), 0, 0, 0, 18, 23, 0, HOUR_FLAG|MINUTE_FLAG},
    {string("18:58 am"), 0, 0, 0, 18, 58, 0, HOUR_FLAG|MINUTE_FLAG}
};

bool timePareCmp(ptm pTime,uint32_t parseFlag, timeDetail *detail)
{
    time_t tt;
    struct tm *nowTime;
    tt = time(NULL);
    nowTime = localtime(&tt);
    if ((detail->flag&parseFlag)!=detail->flag)
    {
        printf("error timePareCmp flag %X VS %X\n", detail->flag, parseFlag);
        return false;
    }

    if ((detail->flag&YEAR_FLAG) && (pTime->tm_year+1900)!=detail->year)
    {
        printf("error year %d should be %d\n", pTime->tm_year+1900, detail->year);
        return false;
    } else if (!(detail->flag&YEAR_FLAG) && pTime->tm_year!=nowTime->tm_year)
    {
        printf("erro year should be now");
        return false;
    }

    if ((detail->flag&MONTH_FLGA) && pTime->tm_mon!=detail->month)
    {
        printf("error month %d should be %d\n", pTime->tm_mon, detail->month);
        return false;
    } else if (!(detail->flag&MONTH_FLGA) && pTime->tm_mon!=nowTime->tm_mon)
    {
        printf("error month should be now (is %d)", nowTime->tm_mon);
        return false;
    }
    if ((detail->flag&DAY_FLAG) && pTime->tm_mday!=detail->day)
    {
        printf("error day %d should be %d\n", pTime->tm_mday, detail->day);
        return false;
    } else if (!(detail->flag&DAY_FLAG) && pTime->tm_mday!=nowTime->tm_mday)
    {
        printf("error day should be now");
        return false;
    }

    if ((detail->flag&HOUR_FLAG) && pTime->tm_hour!=detail->hour)
    {
        printf("error hour %d should be %d\n", pTime->tm_hour, detail->hour);
        return false;
    }
    if ((detail->flag&MINUTE_FLAG) && pTime->tm_min!=detail->minute)
    {
        printf("error minute %d should be %d\n", pTime->tm_min, detail->minute);
        return false;
    }
    if ((detail->flag&SECOND_FLAG) && pTime->tm_sec!=detail->second)
    {
        printf("error second %d should be %d\n", pTime->tm_sec, detail->second);
        return false;
    }

    return true;
}

pParseTime parseFunction;

TEST(parse, sp)
{
    timeDetail detail = {string("2020-2-1"), 2021, 1, 1, 9, 0, 0, YEAR_FLAG|MONTH_FLGA|DAY_FLAG};

    printf("send to parse:%s\r\n", detail.sentence.c_str());
    int32_t ret;
    uint32_t flag = 0;
    struct tm timeGot;
    memset(&timeGot, 0, sizeof(timeGot));
    ret = parseFunction(detail.sentence.c_str(), (uint32_t)detail.sentence.length(), &timeGot, &flag);
    printf("ret = %d time is %d-%d-%d ", ret, timeGot.tm_year+1900, timeGot.tm_mon ,timeGot.tm_mday);
    printf("%d:%d:%d\n", timeGot.tm_hour, timeGot.tm_min ,timeGot.tm_sec);
}

TEST(parse, formate)
{
    int32_t ret;
    uint32_t flag = 0;

    for (auto &it:formateTestCase)
    {
        printf("send to parse:%s\r\n", it.sentence.c_str());
        struct tm timeGot;
        flag = 0;
        ret = parseFunction(it.sentence.c_str(), (uint32_t)it.sentence.length(), &timeGot, &flag);
        ASSERT_GE(ret, 1) ;
        ASSERT_TRUE(timePareCmp(&timeGot, flag, &it));
    }
}

int main(int argc, char* argv[]) {
    void *tpHandle;
    std::string pluginName = string(PLAG_NAME) + "." + PLAG_SUBFIX;
    tpHandle = dlopen(pluginName.c_str(), RTLD_LAZY);
    if (!tpHandle)
    {
		fprintf(stderr, "fail to load plugin: %s for %s\n", pluginName.c_str(), dlerror());
        return -1;
    }
    parseFunction = (pParseTime)dlsym(tpHandle, "parseTime");
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 
    dlclose(tpHandle);
    return ret;
}