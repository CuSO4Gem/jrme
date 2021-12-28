#include "date/date.h"
using namespace date;
#include <time.h>
#include "timeparser_plugin.h"
#include "parseRegex.h"

using namespace std;
using namespace std::chrono;

#define ALL_FULL_MONTH_REGEX "(january|february|march|april|may|june|july|august|september|october|november|december)"
#define ALL_SHORT_MONTH_REGEX "(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)"

// static const char monthFullName[12][] = {
// 	"january",
// 	"february",
// 	"march",
// 	"april",
// 	"may",
// 	"june",
// 	"july",
// 	"august",
// 	"september",
// 	"october",
// 	"november",
// 	"december",
// };

// static const char *monthShortName[12][] = {
// 	"jan",
// 	"feb",
// 	"mar",
// 	"apr",
// 	"may",
// 	"jun",
// 	"jul",
// 	"aug",
// 	"sep",
// 	"oct",
// 	"nov",
// 	"dec",
// };

int32_t parseRegex_normalDescribe(ptm ptime)
{

    year_month_day today = floor<days>(system_clock::now()-days(26));
    ptime->tm_sec = 20;
    
    return 4;
}