#ifndef JOURNAL_IO_FACTORY
#define JOURNAL_IO_FACTORY

#include <memory>
#include <string>

#include "TxtJournalIO.h"
#include "JournalIOBase.h"
#include "JrmeConfig.h"
#include "PluginJournalIO.h"

using namespace std;

class JournalIOFactory
{
private:
    /* data */
public:
    JournalIOFactory() = default;
    ~JournalIOFactory() = default;

    shared_ptr<JournalIOBase> getJournalIO(string journalPath);
};


#endif