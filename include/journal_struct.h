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
#pragma GCC visibility push(default)

void releaseJournalStruct(struct journal_s journal)
{
    if (journal.title)
        free(journal.title);
    if (journal.config)
        free(journal.config);
    if (journal.content)
        free(journal.content);
}

#pragma GCC visibility pop
#ifdef __cplusplus
}
#endif

#endif