#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <string>

#include "timeparser_plugin.h"

using namespace std;

#define PLAG_NAME "timeparser_en"
#define PLAG_SUBFIX "so"

#include "timeparser_plugin.h"
int main(int argc, char* argv[]) {
    void *tpHandle;
    std::string pluginName = string(PLAG_NAME) + "." + PLAG_SUBFIX;
    
    tpHandle = dlopen(pluginName.c_str(), RTLD_LAZY);
    if (!tpHandle)
    {
		fprintf(stderr, "fail to load plugin: %s for %s\n", pluginName.c_str(), dlerror());
        return -1;
    }
    pParseTime parse = (pParseTime)dlsym(tpHandle, "parseTime");
    int32_t ret;
    uint32_t flag = 0;
    std::string time_str = "in   today  \ton thO At am11 am ammm";
    printf("send to parse:%s\r\n",time_str.c_str());
    struct tm timeGot;
    memset(&timeGot, 0, sizeof(timeGot));
    ret = parse(time_str.c_str(), (uint32_t)time_str.length(), &timeGot, &flag);
    printf("time is %d-%d-%d, sec=%d\n", timeGot.tm_year+1900, timeGot.tm_mon ,timeGot.tm_mday, timeGot.tm_sec);
    dlclose(tpHandle);
    return 0;
}