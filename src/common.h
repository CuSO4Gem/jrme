#ifndef COMMON_H
#define COMMON_H

#ifdef WIN32
    #ifdef DLL_EXPORTS
        #define CJRNL_API __declspec(dllexport)
    #else
        #define CJRNL_API
    #endif // _MSC_VER
#else
    #ifdef DLL_EXPORTS
        #define CJRNL_API __attribute__((visibility("default")))
    #else
        #define CJRNL_API
    #endif
#endif


#endif