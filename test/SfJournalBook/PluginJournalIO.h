#ifndef PLUGIN_JOURNAL_IO_H
#define PLUGIN_JOURNAL_IO_H

#include "Journal.h"

#include "JournalIOBase.h"
#include "journal_io_api.h"

class PluginJournalIO : public JournalIOBase
{
private:
    string mJournalPath;

    void* mDlHandle = NULL;
    void* mJournalIOHandle = NULL;
    uint32_t mApiSupport;
    vector<string> mFormate;

    releaseJournal_fnc p_releaseJournal = NULL;

    allocate_instance_fnc p_allocate_instance = NULL;
    release_instance_fnc p_release_instance = NULL;

    apiSupport_fnc p_apiSupport = NULL;
    formateSupport_fnc p_formateSupport = NULL;
    isSupportAes256_fnc p_isSupportAes256 = NULL;
    setKey_fnc p_setKey = NULL;
    clearKey_fnc p_clearKey = NULL;
    openIO_fnc p_openIO = NULL;
    closeIO_fnc p_closeIO = NULL;
    readMode_fnc p_readMode = NULL;
    writeMode_fnc p_writeMode = NULL;
    readJournal_fnc p_readJournal = NULL;
    writeJournal_fnc p_writeJournal = NULL;

public:
    PluginJournalIO() = default;
    ~PluginJournalIO();

    bool loadPlugin(string name);

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