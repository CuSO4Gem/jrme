#ifndef CONFIG_NODE_API_H
#define CONFIG_NODE_API_H

#include <stdint.h>

struct journalIn_s
{
    const char *title;
    const char *config;
    char *content;
};

struct journal_s
{
    char *title;
    char *config;
    char *content;
};

#endif