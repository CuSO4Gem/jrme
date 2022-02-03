#ifndef SFJOURNAL_BOOK_BASE_H
#define SFJOURNAL_BOOK_BASE_H

#include <list>
#include <memory>

#include "Journal.h"
#include "JournalBookBase.h"
#include "JournalIOBase.h"
#include "TxtJournalIO.h"

class SfJournalBook : public JournalBookBase
{
private:
    shared_ptr<JournalIOBase> mJournalIO;

public:
    SfJournalBook() = default;
    ~SfJournalBook();

    bool open(string path);
    void sort(int32_t type);
    bool save();
};
#endif