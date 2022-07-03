/* MIT License
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/////////////////////////////////////////
//// Configurations
/////////////////////////////////////////

#ifndef DBG_PRINT_ENABLED
/**
 * Enable debug printing
 */
#define DBG_PRINT_ENABLED (1)
//0:no log, 1:error, 2:warning, 3:trace, 4.Debug
#endif /* DBG_PRINT_ENABLED */
/*file remove path*/
#define MYFILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x

#ifndef DGB_PRINT_BUFFER_SIZE
/**
 * How large can the debug message be in characters
 */
#define DGB_PRINT_BUFFER_SIZE (512)
#endif /* DGB_PRINT_BUFFER_SIZE */

#ifndef DBG_PRINT_BUFFER_TOO_SMALL
/**
 * Error code for the debug buffer being too small
 */
#define DBG_PRINT_BUFFER_TOO_SMALL (0xDEAD)
#endif /* DBG_PRINT_BUFFER_TOO_SMALL */

////////////////////////////////////////
//// Callbacks
////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \brief Callback function to be implemented by the user.
*        This callback function will be called to send the debug message out.
*        You may implement your usage of the debug printing, be that
*        printing to a UART port or logging to a file.
*/
extern int debug_print_callback(char *debugMessage, unsigned int length);

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////
//// DebugPrint-API
/////////////////////////////////////////


#ifdef __cplusplus
extern "C"
{
#endif

static int debug_printf_ln(const char *color, const char *fileName, const char *funcName, unsigned int lineNumber, const char *fmt, ...)
{
    char parameterBuffer[DGB_PRINT_BUFFER_SIZE] = {0};
    char outgoingBuffer[DGB_PRINT_BUFFER_SIZE] = {0};
    va_list params;

    va_start(params, fmt);
    vsnprintf(parameterBuffer, DGB_PRINT_BUFFER_SIZE, fmt, params);
    va_end(params);

    int ret = snprintf(outgoingBuffer, DGB_PRINT_BUFFER_SIZE, "%s[%s:%s():%u]: %s%s", color, fileName, funcName, lineNumber, parameterBuffer, ANSI_COLOR_RESET);
    if (ret < 0)
    {
        return DBG_PRINT_BUFFER_TOO_SMALL;
    }

    printf("%s\n",outgoingBuffer);
    return 0;
}

#if DBG_PRINT_ENABLED >= (1)
#define JLOGE(fmt, ...) debug_printf_ln(ANSI_COLOR_RED, MYFILE(__FILE__), __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define JLOGE(fmt, ...)
#endif

#if DBG_PRINT_ENABLED >= (2)
#define JLOGW(fmt, ...) debug_printf_ln(ANSI_COLOR_YELLOW, MYFILE(__FILE__), __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define JLOGW(fmt, ...)
#endif

#if DBG_PRINT_ENABLED >= (3)
#define JLOGT(fmt, ...) debug_printf_ln(ANSI_COLOR_BLUE, MYFILE(__FILE__), __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define JLOGT(fmt, ...)
#endif

#if DBG_PRINT_ENABLED >= (4)
#define JLOGD(fmt, ...) debug_printf_ln(ANSI_COLOR_GREEN, MYFILE(__FILE__), __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define JLOGD(fmt, ...)
#endif

#ifdef __cplusplus
}
#endif
#endif /* _DEBUG_PRINT_H_ */
