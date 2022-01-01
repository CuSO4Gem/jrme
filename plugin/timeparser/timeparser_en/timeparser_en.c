#include "timeparser_en.h"
#include "parseRegex.h"

NOIT_IT_API int32_t parseTime(const char *str, uint32_t len, ptm ptime, uint32_t *flag)
{
    if (str==NULL || len <=0 || ptime==NULL || flag==NULL)
    {
        return TIME_PARSE_PARAMETER_ERROR;
    }
    
    ptime->tm_sec = 12;
    parseRegex(str, 6,ptime);
    return TIME_PARSE_LITTLE_SUCCESS;
}
