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

#include "JAttributeMaster.h"
#include "JAttributeBase.h"
#include "DataJAttribute.h"
#include "debug_print.h"
#include "LevelJAttribute.h"
#include "TagJAttribute.h"

#include "jattribute_plugin_api.h"
#include "journal_struct.h"

/**
 * @brief 用于加载JAttribute插件
 * To load JAttribute pulgin.
 */
class JAttributePlugin : public JAttributeBase
{
private:
    void* mDlHandle = NULL;
    void* mHandle = NULL;
    allocate_instance_fnc p_allocate = NULL;
    release_instance_fnc  p_release = NULL;
    preprocess_fnc  p_preprocess = NULL;
    postprocess_fnc  p_postprocess = NULL;
    releaseJournal_fnc p_releaseJournal = NULL;
    uint32_t mApiVersion;
    const char *mKey = NULL;
    const char *mDefaultValue = NULL;

public:
    JAttributePlugin() = default;
    ~JAttributePlugin();

    bool loadPlugin(string name);

    uint32_t apiVersion();
    string getKey();
    string getDefaultValue();
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);
};

JAttributePlugin::~JAttributePlugin()
{
    if (mHandle && p_release)
        p_release(mHandle);

    if (mDlHandle)
        dlclose(mDlHandle);
}

bool JAttributePlugin::loadPlugin(string name)
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

    mHandle = p_allocate();
    if (!mHandle)
    {
        dlclose(mDlHandle);
        return false;
    }
    return true;
}

uint32_t JAttributePlugin::apiVersion()
{
    return mApiVersion;
}

string JAttributePlugin::getKey()
{
    if (!mKey)
        return string();

    return string(mKey);
}

string JAttributePlugin::getDefaultValue()
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
    strBuffer = journal->getAttributePart();
    journalRet.attributePart = (char*)malloc(strBuffer.length()+1);
    memcpy(journalRet.attributePart, strBuffer.c_str(), strBuffer.length()+1);
    strBuffer = journal->getContent();
    journalRet.content = (char*)malloc(strBuffer.length()+1);
    memcpy(journalRet.content, strBuffer.c_str(), strBuffer.length()+1);
    return journalRet;
}

void JAttributePlugin::preprocess(shared_ptr<Journal> journal)
{
    struct journal_s orgJournal = journal2S(journal);
    
    string title = journal->getTitle();
    string attributePart = journal->getAttributePart();
    string content = journal->getContent();
    struct journal_s retJournal = {NULL, NULL, NULL};
    p_preprocess(mHandle, &orgJournal, &retJournal);
    if (retJournal.title != NULL)
    {
        string title = string(retJournal.title);
        journal->setTitle(title);
    }
    if (retJournal.attributePart != NULL)
    {
        string attributePart = string(retJournal.attributePart);
        journal->setAttributePart(attributePart);
    }
    if (retJournal.content != NULL)
    {
        string content = string(retJournal.content);
        journal->setContent(content);
    }
    /**
     * 输入的journal_s结构体由jrme释放，输出的由插件释放
     * input journal_s struct is released by jrme, output struct is released by plugin
     */
    releaseJournalStruct(orgJournal);
    p_releaseJournal(retJournal);
    return;
}

void JAttributePlugin::postprocess(shared_ptr<Journal> journal)
{
    struct journal_s orgJournal = journal2S(journal);
    
    string title = journal->getTitle();
    string attributePart = journal->getAttributePart();
    string content = journal->getContent();
    struct journal_s retJournal = {NULL, NULL, NULL};
    p_postprocess(mHandle, &orgJournal, &retJournal);
    if (retJournal.title != NULL)
    {
        string title = string(retJournal.title);
        journal->setTitle(title);
    }
    if (retJournal.attributePart != NULL)
    {
        string attributePart = string(retJournal.attributePart);
        journal->setAttributePart(attributePart);
    }
    if (retJournal.content != NULL)
    {
        string content = string(retJournal.content);
        journal->setContent(content);
    }

    /**
     * 输入的journal_s结构体由jrme释放，输出的由插件释放
     * input journal_s struct is released by jrme, output struct is released by plugin
     */
    releaseJournalStruct(orgJournal);
    p_releaseJournal(retJournal);
    return;
}


JAttributeMaster::JAttributeMaster()
{
    mNodeList.push_back(make_shared<DataJAttribute>());
    mNodeList.push_back(make_shared<TagJAttribute>());
    mNodeList.push_back(make_shared<LevelJAttribute>());
}

size_t JAttributeMaster::attributeSize()
{
    return mNodeList.size();
}

/**
 * @brief 加载一个插件
 * Load a plugin
 * 
 * @param name 插件路径。Path of plugin
 * @return true 加载成功。Load successfully.
 * @return false 加载失败。Load failed.
 */
bool JAttributeMaster::addPluginNode(string name)
{
    shared_ptr<JAttributePlugin> node = make_shared<JAttributePlugin>();
    if (!node->loadPlugin(name))
    {
        return false;
    }
    mNodeList.push_back(node);
    return true;
}

/**
 * @brief 根据加载的JAttribute生成日记中默认的attribute part
 * Generate default attribute part in journal by loaded JAttribute
 * 
 * @return string 
 */
string JAttributeMaster::genJAttributePart()
{
    string attributePart;
    for (auto &it:mNodeList)
    {
        attributePart.append(it->getKey() + "=" + it->getDefaultValue() + "\n");
    }
    return attributePart;
}

void JAttributeMaster::preprocess(shared_ptr<Journal> journal)
{
    for (auto &it:mNodeList)
    {
        it->preprocess(journal);
    }
    return;
}

void JAttributeMaster::postprocess(shared_ptr<Journal> journal)
{
    for (auto &it:mNodeList)
    {
        it->postprocess(journal);
    }
    return;
}

void JAttributeMaster::setDate(time_t stamp)
{
    for (auto &it:mNodeList)
    {
        it->setDate(stamp);
    }
    return;
}
