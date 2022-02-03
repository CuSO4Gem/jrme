#ifndef TXT_JOURNAL_IO
#define TXT_JOURNAL_IO

#include <fstream>

#include "JournalIOBase.h"

enum processState {
    UNINITED,
    INITED,
    READ,
    WRITE
};

class TxtJournalIO : JournalIOBase
{
private:
    fstream mJrounal;
    string mJournalPath;
    size_t mJournalStart;
    enum processState mState;

public:
    TxtJournalIO();
    ~TxtJournalIO();

    uint32_t apiSupport();
    list<string> formateSupport();
    bool setReadMod();
    bool setWriteMode();
    bool openJournal(string path);
    bool readJournal(string &title, string &config, string &content);
    bool writeJournal(const string &title, const string &config, const string &content);
};

#endif