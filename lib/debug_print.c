/* MIT License
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <debug_print.h>

int debug_printf_ln(const char *color, const char *fileName, const char *funcName, unsigned int lineNumber, const char *fmt, ...)
{
    char parameterBuffer[DGB_PRINT_BUFFER_SIZE] = {0};
    char outgoingBuffer[DGB_PRINT_BUFFER_SIZE] = {0};
    va_list params;

    va_start(params, fmt);
    vsnprintf(parameterBuffer, DGB_PRINT_BUFFER_SIZE, fmt, params);
    va_end(params);

    int ret = snprintf(outgoingBuffer, DGB_PRINT_BUFFER_SIZE, "%s[%s:%s():%u]: %s\r\n%s", color, fileName, funcName, lineNumber, parameterBuffer, ANSI_COLOR_RESET);
    if (ret < 0)
    {
        return DBG_PRINT_BUFFER_TOO_SMALL;
    }

    return debug_print_callback(outgoingBuffer, (DGB_PRINT_BUFFER_SIZE < ret) ? DGB_PRINT_BUFFER_SIZE : (unsigned int)ret);
}
