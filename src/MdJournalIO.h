#ifndef MD_JOURNAL_IO_H
#define MD_JOURNAL_IO_H

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
    list<string> formateSupport();
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