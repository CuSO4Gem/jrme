#include "JournalIOFactory.h"

#include "debug_print.h"

shared_ptr<JournalIOBase> JournalIOFactory::getJournalIO(string journalPath)
{
    if (journalPath.length() == 0)
    {
        JLOGW("[W] ournalIOFactory::%s, get null file name", __func__);
        return nullptr;
    }
    
    /*load all journal IO*/
    vector<shared_ptr<JournalIOBase>> ioVector;
    ioVector.push_back(make_shared<TxtJournalIO>());
    JLOGD("[D] load TxtJournal at 0");
    size_t i = 1;
    vector<string> plugins = getJournalIOPluginNames();
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

    /*get postfix*/
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

    /*find postfix, so select journalIO by postfix*/
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
                    return it;
                }
            }
        }
    }

    /*not get postfix or can not open journal by postfix
      so we open journal one by one*/
    for (auto &it:ioVector)
    {
        if (it->open(journalPath))
        {
            it->close();
            return it;
        }
    }

    JLOGW("[W] JournalIOFactory create journalIO failed!");
    return nullptr;
}