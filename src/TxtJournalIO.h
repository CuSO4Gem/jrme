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
#ifndef TXT_JOURNAL_IO_H
#define TXT_JOURNAL_IO_H

#include <fstream>
#include <memory>

#include "Journal.h"
#include "JournalIOBase.h"

enum processState {
    UNINITED,
    INITED,
    READ,
    WRITE
};

/**
 * @brief 
 * txt格式的日记IO
 * IO txt formate journal
 */
class TxtJournalIO : public JournalIOBase
{
private:
    fstream mJournal;
    string mJournalPath;
    enum processState mState;

public:
    TxtJournalIO();
    ~TxtJournalIO();

    uint32_t apiSupport();
    vector<string> formateSupport();
    bool isSupportAes256();
    void setKey(uint8_t key[32]);
    void clearKey();
    
    bool setReadMod();
    bool setWriteMode();
    bool open(string path);
    void close();
    shared_ptr<Journal> readJournal();
    bool writeJournal(shared_ptr<Journal> journal);
};

#endif