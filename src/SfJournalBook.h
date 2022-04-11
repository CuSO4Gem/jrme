/*
Copyright 2022 Zorn Link

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
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

/**
 * @brief 
 * 单文件日记本
 * journal book with single file.
 */
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