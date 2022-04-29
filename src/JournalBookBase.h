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
#ifndef JOURNAL_BOOK_BASE_H
#define JOURNAL_BOOK_BASE_H

#include <memory>
#include "Journal.h"
/**
 * @brief 
 * 日记本的接口类，之后可能会推出多文件模式的日记本
 * 
 * Interface class of journal book. Maybe will have journal book with multy file
 */
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
    virtual void erase(size_t pos) = 0;
    virtual bool swap(size_t pos1, size_t pos2) = 0;
};

#endif