#include "date.h"
#include <time.h>
#include "timeparser_plugin.h"
#include "parseRegex.h"

using namespace ec;
using namespace std;

#define ALL_FULL_MONTH_REGEX "(january|february|march|april|may|june|july|august|september|october|november|december)"
#define ALL_SHORT_MONTH_REGEX "(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)"

constexpr int32_t TWOCC(const char *s)
{
    return  (unsigned char)*(s)|
            (unsigned char)*(s+1)<<8;
}

void parseVM(string &cmdSequence, ptm ptime)
{
    return;
}

int32_t parseRegex(char *str, uint32_t len ,ptm ptime)
{
    Time nowTime;
    Date date = nowTime.toDate();
    date.setMonth(2);
    
    ptime->tm_year = date.year()-1900;
    ptime->tm_mon = date.month();
    ptime->tm_mday = date.day();
    
    
    return 4;
}