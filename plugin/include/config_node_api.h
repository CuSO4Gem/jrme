#ifndef CONFIG_NODE_API_H
#define CONFIG_NODE_API_H

#include <stdint.h>

struct journal_cs
{
    const char *title;
    const char *config;
    const char *content;
};

struct journal_s
{
    char *title;
    char *config;
    char *content;
};


typedef void *(*allocate_instance_fnc)();
typedef void (*release_instance_fnc)(void *);
typedef void (*preprocess_fnc)(void *, const struct journal_cs *, struct journal_s *);
typedef void (*postprocess_fnc)(void *, const struct journal_cs *, struct journal_s *);

#endif