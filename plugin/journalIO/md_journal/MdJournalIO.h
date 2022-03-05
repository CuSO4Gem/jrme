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


uint32_t apiSupport(void *handle);
char* formateSupport(void *handle, size_t *line_num);
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