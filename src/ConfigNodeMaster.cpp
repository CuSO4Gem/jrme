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
#include <dlfcn.h>

#include "JrmeConfig.h"

#include "ConfigNodeMaster.h"
#include "ConfigNodeBase.h"
#include "DateConfigNode.h"
#include "debug_print.h"
#include "LevelConfigNode.h"
#include "TagConfigNode.h"

#include "config_node_api.h"
#include "journal_struct.h"

/**
 * @brief 用于打开config node插件
 * To open config node pulgin.
 */
class PluginNode : public ConfigNodeBase
{
private:
    void* mDlHandle = NULL;
    void* mNodeHandle = NULL;
    allocate_instance_fnc p_allocate = NULL;
    release_instance_fnc  p_release = NULL;
    preprocess_fnc  p_preprocess = NULL;
    postprocess_fnc  p_postprocess = NULL;
    releaseJournal_fnc p_releaseJournal = NULL;
    uint32_t mApiVersion;
    const char *mKey = NULL;
    const char *mDefaultValue = NULL;

public:
    PluginNode() = default;
    ~PluginNode();

    bool loadPlugin(string name);

    uint32_t apiVersion();
    string getKey();
    string getDefaultValue();
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);
};

PluginNode::~PluginNode()
{
    if (mNodeHandle && p_release)
        p_release(mNodeHandle);

    if (mDlHandle)
        dlclose(mDlHandle);
}

bool PluginNode::loadPlugin(string name)
{
    string path = JrmeConfig::getPluginDir()+name;
    mDlHandle = dlopen(path.c_str(), RTLD_NOW);
    if (!mDlHandle)
    {
        JLOGW("[W] dlopen - %s", dlerror());
        return false;
    }
    
    p_allocate = (allocate_instance_fnc)dlsym(mDlHandle, "allocate_instance");
    if (!p_allocate)
    {
        dlclose(mDlHandle);
        return false;
    }
    
    p_release = (release_instance_fnc)dlsym(mDlHandle, "release_instance");
    if (!p_release)
    {
        dlclose(mDlHandle);
        return false;
    }
    
    uint32_t *p_api_version = (uint32_t *)dlsym(mDlHandle, "api_version");
    mApiVersion ? *p_api_version : 1;

    mKey = (char *)dlsym(mDlHandle, "key");
    if (!mKey)
    {
        dlclose(mDlHandle);
        return false;
    }
    mDefaultValue = (char *)dlsym(mDlHandle, "default_value");
    if (!mDefaultValue)
    {
        dlclose(mDlHandle);
        return false;
    }
    
    p_preprocess = (preprocess_fnc)dlsym(mDlHandle, "preprocess");
    if (!p_preprocess)
    {
        dlclose(mDlHandle);
        return false;
    }
    p_postprocess = (postprocess_fnc)dlsym(mDlHandle, "postprocess");
    if (!p_postprocess)
    {
        dlclose(mDlHandle);
        return false;
    }

    p_releaseJournal = (releaseJournal_fnc)dlsym(mDlHandle, "releaseJournalStruct");
    if (!p_releaseJournal)
    {
        dlclose(mDlHandle);
        return false;
    }

    mNodeHandle = p_allocate();
    if (!mNodeHandle)
    {
        dlclose(mDlHandle);
        return false;
    }
    return true;
}

uint32_t PluginNode::apiVersion()
{
    return mApiVersion;
}

string PluginNode::getKey()
{
    if (!mKey)
        return string();

    return string(mKey);
}

string PluginNode::getDefaultValue()
{
    if (!mDefaultValue)
        return string();
    
    return string(mDefaultValue);
}

/**
 * @brief 将journal class转换为journal_s
 * transfer journal class to journal_s
 * 
 * @param journal 
 * @return struct journal_s 
 */
struct journal_s journal2S(shared_ptr<Journal> journal)
{
    struct journal_s journalRet;
    string strBuffer =  journal->getTitle();
    journalRet.title = (char*)malloc(strBuffer.length()+1);
    memcpy(journalRet.title, strBuffer.c_str(), strBuffer.length()+1);
    strBuffer = journal->getConfig();
    journalRet.config = (char*)malloc(strBuffer.length()+1);
    memcpy(journalRet.config, strBuffer.c_str(), strBuffer.length()+1);
    strBuffer = journal->getContent();
    journalRet.content = (char*)malloc(strBuffer.length()+1);
    memcpy(journalRet.content, strBuffer.c_str(), strBuffer.length()+1);
    return journalRet;
}

void PluginNode::preprocess(shared_ptr<Journal> journal)
{
    struct journal_s orgJournal = journal2S(journal);
    
    string title = journal->getTitle();
    string config = journal->getConfig();
    string content = journal->getContent();
    struct journal_s retJournal = {NULL, NULL, NULL};
    p_preprocess(mNodeHandle, &orgJournal, &retJournal);
    if (retJournal.title != NULL)
    {
        string title = string(retJournal.title);
        journal->setTitle(title);
    }
    if (retJournal.config != NULL)
    {
        string config = string(retJournal.config);
        journal->setConfig(config);
    }
    if (retJournal.content != NULL)
    {
        string content = string(retJournal.content);
        journal->setContent(content);
    }
    releaseJournalStruct(orgJournal);
    p_releaseJournal(retJournal);
    return;
}

void PluginNode::postprocess(shared_ptr<Journal> journal)
{
    struct journal_s orgJournal = journal2S(journal);
    
    string title = journal->getTitle();
    string config = journal->getConfig();
    string content = journal->getContent();
    struct journal_s retJournal = {NULL, NULL, NULL};
    p_postprocess(mNodeHandle, &orgJournal, &retJournal);
    if (retJournal.title != NULL)
    {
        string title = string(retJournal.title);
        journal->setTitle(title);
    }
    if (retJournal.config != NULL)
    {
        string config = string(retJournal.config);
        journal->setConfig(config);
    }
    if (retJournal.content != NULL)
    {
        string content = string(retJournal.content);
        journal->setContent(content);
    }

    releaseJournalStruct(orgJournal);
    p_releaseJournal(retJournal);
    return;
}


ConfigNodeMaster::ConfigNodeMaster()
{
    mNodeList.push_back(make_shared<DateConfigNode>());
    mNodeList.push_back(make_shared<TagConfigNode>());
    mNodeList.push_back(make_shared<LevelConfigNode>());
}

size_t ConfigNodeMaster::nodeSize()
{
    return mNodeList.size();
}

bool ConfigNodeMaster::addPluginNode(string name)
{
    shared_ptr<PluginNode> node = make_shared<PluginNode>();
    if (!node->loadPlugin(name))
    {
        return false;
    }
    mNodeList.push_back(node);
    return true;
}

string ConfigNodeMaster::genConfig()
{
    string config;
    for (auto &it:mNodeList)
    {
        config.append(it->getKey() + "=" + it->getDefaultValue() + "\n");
    }
    return config;
}

void ConfigNodeMaster::preprocess(shared_ptr<Journal> journal)
{
    for (auto &it:mNodeList)
    {
        it->preprocess(journal);
    }
    return;
}

void ConfigNodeMaster::postprocess(shared_ptr<Journal> journal)
{
    for (auto &it:mNodeList)
    {
        it->postprocess(journal);
    }
    return;
}

void ConfigNodeMaster::setDate(time_t stamp)
{
    for (auto &it:mNodeList)
    {
        it->setDate(stamp);
    }
    return;
}
