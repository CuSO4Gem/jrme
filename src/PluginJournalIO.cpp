#include <dlfcn.h>
#include <string.h>

#include "PluginJournalIO.h"
#include "JrmeConfig.h"

#include "journal_struct.h"

/*debug*/
#include "debug_print.h"

#define FUNC_PTR_CHECK(ptr) if (!(ptr)) { if (mDlHandle) {dlclose(mDlHandle);} return false; }

PluginJournalIO::~PluginJournalIO()
{
    if (mJournalIOHandle && p_release_instance)
        p_release_instance(mJournalIOHandle);
    
    if (mDlHandle)
        dlclose(mDlHandle);
}

bool PluginJournalIO::loadPlugin(string name)
{
    string path = getPluginDir()+name;
    mDlHandle = dlopen(path.c_str(), RTLD_NOW);
    if (!mDlHandle)
    {
        JLOGW("dlopen - %s\n", dlerror());
        return false;
    }
    p_releaseJournal = (releaseJournal_fnc)dlsym(mDlHandle, "releaseJournalStruct");
    FUNC_PTR_CHECK(p_releaseJournal)
    p_allocate_instance = (allocate_instance_fnc)dlsym(mDlHandle, "allocate_instance");
    FUNC_PTR_CHECK(p_allocate_instance)
    p_release_instance = (release_instance_fnc)dlsym(mDlHandle, "release_instance");
    FUNC_PTR_CHECK(p_release_instance)
    p_apiSupport = (apiSupport_fnc)dlsym(mDlHandle, "apiSupport");
    FUNC_PTR_CHECK(p_apiSupport)
    p_formateSupport = (formateSupport_fnc)dlsym(mDlHandle, "formateSupport");
    FUNC_PTR_CHECK(p_formateSupport)
    p_isSupportAes256 = (isSupportAes256_fnc)dlsym(mDlHandle, "isSupportAes256");
    FUNC_PTR_CHECK(p_isSupportAes256)
    p_setKey = (setKey_fnc)dlsym(mDlHandle, "setKey");
    FUNC_PTR_CHECK(p_setKey)
    p_clearKey = (clearKey_fnc)dlsym(mDlHandle, "clearKey");
    FUNC_PTR_CHECK(p_clearKey)
    p_openIO = (openIO_fnc)dlsym(mDlHandle, "openIO");
    FUNC_PTR_CHECK(p_openIO)
    p_closeIO = (closeIO_fnc)dlsym(mDlHandle, "closeIO");
    FUNC_PTR_CHECK(p_closeIO)
    p_readMode = (readMode_fnc)dlsym(mDlHandle, "setReadMod");
    FUNC_PTR_CHECK(p_readMode)
    p_writeMode = (writeMode_fnc)dlsym(mDlHandle, "setWriteMode");
    FUNC_PTR_CHECK(p_writeMode)
    p_readJournal = (readJournal_fnc)dlsym(mDlHandle, "readJournal");
    FUNC_PTR_CHECK(p_readJournal)
    p_writeJournal = (writeJournal_fnc)dlsym(mDlHandle, "writeJournal");
    FUNC_PTR_CHECK(p_writeJournal)
    
    mJournalIOHandle = p_allocate_instance();
    if (!mJournalIOHandle)
        return false;

    mApiSupport = p_apiSupport(mJournalIOHandle);
    size_t lines;
    const char *fmt;
    fmt = p_formateSupport(mJournalIOHandle, &lines);
    if (fmt)
    {
        for (size_t i = 0; i < lines; i++)
        {
            mFormate.push_back(string(fmt+i*FORMATE_TABLE_COLUM));
        }
    }
    return true;
}


uint32_t PluginJournalIO::apiSupport()
{
    if (mJournalIOHandle)
        return mApiSupport;
    
    return 0;
}

vector<string> PluginJournalIO::formateSupport()
{
    return mFormate;
}

bool PluginJournalIO::isSupportAes256()
{
    if (mJournalIOHandle && p_isSupportAes256)
        return p_isSupportAes256(mJournalIOHandle);
    
    return false;
}

void PluginJournalIO::setKey(uint8_t key[32])
{
    if (mJournalIOHandle && p_setKey)
        p_setKey(mJournalIOHandle, key);
    
    return;
}

void PluginJournalIO::clearKey()
{
    if (mJournalIOHandle && p_clearKey)
        p_clearKey(mJournalIOHandle);

    return;
}

bool PluginJournalIO::setReadMod()
{
    if (mJournalIOHandle && p_readMode)
        return p_readMode(mJournalIOHandle);
    
    return false;
}

bool PluginJournalIO::setWriteMode()
{
    if (mJournalIOHandle && p_writeMode)
        return p_writeMode(mJournalIOHandle);
    
    return false;
}

bool PluginJournalIO::open(string path)
{
    if (mJournalIOHandle && p_openIO)
    {
        JLOGD("[D] open file:%s", path.c_str());
        return p_openIO(mJournalIOHandle, path.c_str());
    }
    
    return false;
}

void PluginJournalIO::close()
{
    if (mJournalIOHandle && p_closeIO)
        return p_closeIO(mJournalIOHandle);
}

shared_ptr<Journal> PluginJournalIO::readJournal()
{
    if (!mJournalIOHandle || !p_openIO)
    {
        JLOGE("[E] %s call fail for plugin load fail", __func__);
        return nullptr;
    }
    
    struct journal_s journalRet;
    if (!p_readJournal(mJournalIOHandle, &journalRet))
    {
        JLOGD("[D] readJournal return false, so return nullptr");
        return nullptr;
    }
    
    shared_ptr<Journal> journal = make_shared<Journal>();
    if (journalRet.title)
        journal->setTitle(string(journalRet.title));
    
    if (journalRet.config)
        journal->setConfig(string(journalRet.config));
    
    if (journalRet.content)
        journal->setContent(string(journalRet.content));
    
    p_releaseJournal(journalRet);
    return journal;
}

bool PluginJournalIO::writeJournal(shared_ptr<Journal> journal)
{
    struct journal_s journal2w;
    string stringBuffer = journal->getTitle();
    journal2w.title = (char*)malloc(stringBuffer.length()+1);
    memcpy(journal2w.title, stringBuffer.c_str(), stringBuffer.length()+1);

    stringBuffer = journal->getConfig();
    journal2w.config = (char *)malloc(stringBuffer.length()+1);
    memcpy(journal2w.config, stringBuffer.c_str(), stringBuffer.length()+1);

    stringBuffer = journal->getContent();
    journal2w.content = (char *)malloc(stringBuffer.length()+1);
    memcpy(journal2w.content, stringBuffer.c_str(), stringBuffer.length()+1);

    bool ret = p_writeJournal(mJournalIOHandle, &journal2w);
    releaseJournalStruct(journal2w);
    return ret;
}
