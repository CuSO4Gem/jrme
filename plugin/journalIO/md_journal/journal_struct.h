#ifndef JOURNAL_STRUCT_H
#define JOURNAL_STRUCT_H

#include <malloc.h>

struct journal_s
{
    char *title;
    char *config;
    char *content;
};

typedef void (*releaseJournal_fnc)(struct journal_s);

#ifdef __cplusplus
extern "C" {
#endif

void releaseJournalStruct(struct journal_s journal);

#ifdef __cplusplus
}
#endif

#endif