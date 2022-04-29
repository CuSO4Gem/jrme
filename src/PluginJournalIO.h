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
#ifndef PLUGIN_JOURNAL_IO_H
#define PLUGIN_JOURNAL_IO_H

#include "Journal.h"

#include "JournalIOBase.h"
#include "journal_io_api.h"

/**
 * @brief 
 * 加载和使用JournalIO插件
 * load and use JournalIO plugin
 */
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