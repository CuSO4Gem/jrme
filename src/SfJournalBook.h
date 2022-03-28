#ifndef SFJOURNAL_BOOK_BASE_H
#define SFJOURNAL_BOOK_BASE_H

#include <list>
#include <vector>
#include <memory>
#include <mutex>

#include "Journal.h"
#include "JournalBookBase.h"
#include "JournalIOBase.h"
#include "TxtJournalIO.h"

struct fastSortData
{
    time_t stamp;
    size_t order;
};

class SfJournalBook : public JournalBookBase
{
private:
    shared_ptr<JournalIOBase> mJournalIO;
    vector<shared_ptr<Journal>> mJournalVector;
    static bool fastSortDataCmp(const fastSortData &d1,const fastSortData &d2);

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
    void erase(size_t pos);
    bool swap(size_t pos1, size_t pos2);
};
#endif