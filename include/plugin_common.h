#ifndef PLUGIN_COMMON_H
#define PLUGIN_COMMON_H

#ifdef WIN32
    #ifdef DLL_EXPORTS
        #define JRME_EXPORT __declspec(dllexport)
    #else
        #define JRME_EXPORT
    #endif // _MSC_VER
#else
    #ifdef DLL_EXPORTS
        #define JRME_EXPORT __attribute__((visibility("default")))
    #else
        #define JRME_EXPORT
    #endif
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#endif