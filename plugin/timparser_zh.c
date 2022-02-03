#include "timeparser_plugin.h"
#include "common.h"

CJRNL_API int32_t parseTime(const char *str, int32_t len, ptm ptime, uint32_t *flag)
{
    if (str==NULL || len <=0 || ptime==NULL || flag==NULL)
    {
        return TIME_PARESS_PARAMETER_ERROR;
    }
    ptime->tm_sec = 10;
    return TIME_PARESS_SUCCESS;
}
