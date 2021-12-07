#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <string>

#include "timeparser_plugin.h"

using namespace std;

#define PLAG_NAME "zh_timparser"
#define PLAG_SUBFIX "so"

#include "timeparser_plugin.h"
int main(int argc, char* argv[]) {
    void *tpHandle;
    std::string pluginName = PLAG_NAME + "." + PLAG_SUBFIX;
    
    tpHandle = dlopen(pluginName.c_str(), RTLD_LAZY);
    if (!tpHandle)
    {
		fprintf(stderr, "fail to load plugin: %s\n", plugin_name.c_str());
        return -1;
    }
    pParseTime parse = (pParseTime)dlsym(tpHandle, "parseTime");
    int32_t ret;
    uint32_t flag = 0;
    std::string time_str = "today";
    struct tm timeOut;
    memset(timeOut, 0, sizeof(timeOut));
    ret = parse(time_str.c_str(), time_str.length(), &timeOut, &flag);
    dlclose(tpHandle);
    return 0;
}