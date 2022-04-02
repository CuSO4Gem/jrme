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
#ifndef JOURNAL_IO_BASE_H
#define JOURNAL_IO_BASE_H

#include <memory>
#include <vector>
#include <string>

#include "Journal.h"

using namespace std;

class JournalIOBase
{
public:
    virtual ~JournalIOBase() = default;

    /**
     * @brief support api version
     * 
     * @return uint32_t api version
     */
    virtual uint32_t apiSupport() = 0;

    /**
     * @brief support format
     * 
     * @return vector<string> formate list
     */
    virtual vector<string> formateSupport() = 0;

    /**
     * @brief is IO support Aes-256 encryption
     * 
     * @return true 
     * @return false 
     */
    virtual bool isSupportAes256() = 0;

    /**
     * @brief Set the Key object to object
     * 
     * @param key 
     */
    virtual void setKey(uint8_t key[32]) = 0;

    /**
     * @brief clear key
     * 
     */
    virtual void clearKey() = 0;

    /**
     * @brief open file and ready to parse
     * 
     * @param path the journal path
     * @return true open success
     * @return false open failed
     */
    virtual bool open(string path) = 0;

    /**
     * @brief close IO file
     * 
     */
    virtual void close() = 0;

    /**
     * @brief set read mod, it can read journal
     * 
     * @return true 
     * @return false 
     */
    virtual bool setReadMod() = 0;

    /**
     * @brief set write mod, it can write journal
     * 
     * @return true 
     * @return false 
     */
    virtual bool setWriteMode() = 0;
    /**
     * @brief read and parser from file
     * @return nullptr: read fail
     * @return not nllptr: read success
     */
    virtual shared_ptr<Journal> readJournal() = 0;

    /**
     * @brief write journal to file
     * @return true 
     * @return false 
     */
    virtual bool writeJournal(shared_ptr<Journal> journal) = 0;
};
#endif