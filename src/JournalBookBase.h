#ifndef JOURNAL_BOOK_BASE_H
#define JOURNAL_BOOK_BASE_H

#include <memory>
#include "Journal.h"

class JournalBookBase
{
public:
    virtual ~JournalBookBase() = default;

    virtual bool open(string path) = 0;
    virtual void sort(int32_t type) = 0;
    virtual bool save() = 0;
    
    list<shared_ptr<Journal>> mJournalList;
};

#endif