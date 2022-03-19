#include "journal_struct.h"

void releaseJournalStruct(struct journal_s journal)
{
    if (journal.title)
        free(journal.title);
    if (journal.config)
        free(journal.config);
    if (journal.content)
        free(journal.content);
}