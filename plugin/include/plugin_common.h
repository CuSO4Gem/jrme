#ifndef PLUGIN_COMMON_H
#define PLUGIN_COMMON_H

#define JRME_EXPORT __attribute__((visibility("default")))


#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#endif