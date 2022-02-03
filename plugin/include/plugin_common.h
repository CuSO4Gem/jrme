#ifndef PLUGIN_COMMON_H
#define PLUGIN_COMMON_H

#ifdef WIN32
    #ifdef DLL_EXPORTS
        #define NOIT_IT_API __declspec(dllexport)
    #else
        #define NOIT_IT_API
    #endif // _MSC_VER
#else
    #ifdef DLL_EXPORTS
        #define NOIT_IT_API __attribute__((visibility("default")))
    #else
        #define NOIT_IT_API
    #endif
#endif


#endif