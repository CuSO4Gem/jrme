#include "SfJournalBook.h"

bool SfJournalBook::open(string path)
{
    //todo : add more formate of JournalIO
    mJournalIO = shared_ptr<JournalIOBase>(new TxtJournalIO());
    if (!mJournalIO->open(path))
        return false;
    
    shared_ptr<Journal> journal;
    while ((journal=mJournalIO->readJournal()) != nullptr)
    {
        mJournalList.push_back(journal);
    }
    
    return true;
}

void SfJournalBook::sort(int32_t type)
{
    ;
}

bool SfJournalBook::save()
{
    return true;
}
