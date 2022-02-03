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
    size_t title_size;
    size_t config_size;
    size_t content_size;
};

struct allocate_ret
{
    bool allocated;
    long pre_title_need_more;
    long post_title_need_more;
    long pre_config_need_more;
    long post_config_need_more;
    long pre_content_need_more;
    long post_content_need_more;
};


typedef struct allocate_ret (*allocate_instance_fnc)(void *);
typedef bool (*release_instance_fnc)(void *);
typedef void (*preprocess_fnc)(void *, const struct journal_cs *, struct journal_s *);
typedef void (*postprocess_fnc)(void *, const struct journal_cs *, struct journal_s *);

#endif