#ifndef JOURNAL_BOOK_BASE_H
#define JOURNAL_BOOK_BASE_H

#include <memory>
#include "Journal.h"

class JournalBookBase
{
public:
    virtual ~JournalBookBase() = default;

    virtual void setKey(uint8_t key[32]) = 0;
    virtual bool open(string path) = 0;
    virtual void close() = 0;
    virtual void order() = 0;
    virtual bool save() = 0;
    
    virtual size_t size() = 0;
    virtual shared_ptr<Journal> at(size_t pos) = 0;
    virtual shared_ptr<Journal> operator [](size_t pos) = 0;
    virtual bool insert(size_t pos, shared_ptr<Journal> journal) = 0;
    virtual void push_front(shared_ptr<Journal> journal) = 0;
    virtual void push_back(shared_ptr<Journal> journal) = 0;
    virtual bool swap(size_t pos1, size_t pos2) = 0;
};

#endif