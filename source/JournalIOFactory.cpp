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
#include "JournalIOFactory.h"

#include "debug_print.h"

shared_ptr<JournalIOBase> JournalIOFactory::getJournalIO(string journalPath)
{
    if (journalPath.length() == 0)
    {
        JLOGW("[W] ournalIOFactory::%s, get null file name", __func__);
        return nullptr;
    }
    
    /** 加载所有的JournalIO 
     * Load all JournalIO */
    vector<shared_ptr<JournalIOBase>> ioVector;
    ioVector.push_back(make_shared<TxtJournalIO>());
    JLOGD("[D] load TxtJournal at 0");
    size_t i = 1;
    list<string> plugins = JrmeConfig::getJournalIOPluginNames();
    for (auto &it:plugins)
    {
        shared_ptr<PluginJournalIO> pluginIO = make_shared<PluginJournalIO>();
        if (pluginIO->loadPlugin(it))
        {
            JLOGD("[D] load plaugin %s at %d", it.c_str(), i);
            i++;
            ioVector.push_back(pluginIO);
        }
    }

    /** 获取日记路径的和后缀名
     * Get journal path and suffix */
    size_t pos;
    for (pos = journalPath.length()-1; pos >= 0; pos--)
    {
        if (journalPath[pos] == '.')
            break;
    }
    string postfix = string();
    if (pos>0 && pos<journalPath.length()-1)
    {
        postfix = journalPath.substr(pos+1, journalPath.length()-pos-1);
    }

    /** 优先选择支持该后缀的JournalIO。
     * Select JournalIO which support this postfix. */
    i=0;
    if (postfix.length()>0)
    {
        for (auto &it:ioVector)
        {
            vector<string> formates = it->formateSupport();
            for (auto &itf:formates)
            {
                if (itf==postfix && it->open(journalPath))
                {
                    it->close();
                    JLOGD("[D] match postfix return journalIO %d", i);
                    return it;
                }
            }
            i++;
        }
    }

    /** 没有支持该后缀的JournalIO，所以依次尝试所有的JournalIO
     *  No JournalIO support this postfix, so try all JournalIO */
    i=0;
    for (auto &it:ioVector)
    {
        if (it->open(journalPath))
        {
            JLOGD("[D] not match postfix return journalIO %d", i);
            it->close();
            return it;
        }
        i++;
    }

    JLOGW("[W] JournalIOFactory create journalIO failed!");
    return nullptr;
}