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
#ifndef MD_JOURNAL_IO_H
#define MD_JOURNAL_IO_H

#include <fstream>
#include <memory>

#include "Journal.h"
#include "JournalIOBase.h"
#include "journal_io_api.h"

enum processState {
    UNINITED,
    INITED,
    READ,
    WRITE
};

class MdJournalIO : public JournalIOBase
{
private:
    fstream mJournal;
    string mJournalPath;
    size_t mFileSize;
    enum processState mState;

public:
    MdJournalIO();
    ~MdJournalIO();

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

char formateList[1][FORMATE_TABLE_COLUM] = {"md"};

#ifdef __cplusplus
extern "C" {
#endif
#pragma GCC visibility push(default)

void *allocate_instance();
void release_instance(void *handle);

void releaseJournalStruct(struct journal_s journal);

uint32_t apiSupport(void *handle);
const char* formateSupport(void *handle, size_t *line_num);
bool isSupportAes256(void *handle);
void setKey(void *handle, uint8_t key[32]);
void clearKey(void *handle);

bool setReadMod(void *handle);
bool setWriteMode(void *handle);
bool openIO(void *handle, const char* path);
void closeIO(void *handle);
bool readJournal(void *handle, struct journal_s* journal2R);
bool writeJournal(void *handle, struct journal_s* journal2W);

#pragma GCC visibility pop
#ifdef __cplusplus
}
#endif

#endif