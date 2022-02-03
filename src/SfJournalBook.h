#ifndef SFJOURNAL_BOOK_BASE_H
#define SFJOURNAL_BOOK_BASE_H

#include <list>
#include <memory>
#include <mutex>

#include "AutoLock.h"
#include "Journal.h"
#include "JournalBookBase.h"
#include "JournalIOBase.h"
#include "TxtJournalIO.h"

class SfJournalBook : public JournalBookBase
{
private:
    shared_ptr<JournalIOBase> mJournalIO;
    list<shared_ptr<Journal>> mJournalList;
    mutex mJournalListLock;

public:
    SfJournalBook() = default;
    ~SfJournalBook() = default;

    void setKey(uint8_t key[32]);
    bool open(string path);
    void close();
    void order();
    bool save();

    size_t size();
    shared_ptr<Journal> at(size_t pos);
    shared_ptr<Journal> operator [](size_t pos);
    bool insert(size_t pos, shared_ptr<Journal> journal);
    void push_front(shared_ptr<Journal> journal);
    void push_back(shared_ptr<Journal> journal);
    bool swap(size_t pos1, size_t pos2);
};
#endif