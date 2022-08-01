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
#include <iostream>
#include <list>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "JrmeConfig.h"
#include "Utils.h"
#include "debug_print.h"

using namespace std;

string JrmeConfig::getConfigRootDir()
{
    char const *home = getenv("HOME");
    string      dir  = string(home) + string("/.jrme/");
    return dir;
}

/**
 * @brief 没有任何配置信息情况下的日记本路径。
 * The default journal book path when no config file exists.
 * 
 * @return string 
 */
string JrmeConfig::getInitJournalBookPath()
{
    return JrmeConfig::getConfigRootDir() + string("journal.txt");
}

string JrmeConfig::getJournalBooksCfgPath()
{
    return getConfigRootDir() + string("booklist.cfg");
}

string JrmeConfig::getConfigFilePath()
{
    return getConfigRootDir() + string("config.ini");
}

/**
 * @brief 将'\t'转换为空格，'~'转换为绝对路径
 * Convert '\t' to space, '~' to absolute path
 *
 * @param[in] pathIn
 * @param[out] normalizedPath
 * @return true normalized success
 * @return false normalize fail
 */
bool JrmeConfig::pathNormalize(string &rawPath, string &normalizedPath)
{
    if (rawPath.empty())
        return false;

    normalizedPath = rawPath;
    size_t pos;

    while ((pos = normalizedPath.find('\t')) != string::npos)
    {
        normalizedPath.replace(pos, 1, " ");
    }
    while (normalizedPath[0] == ' ')
    {
        normalizedPath.erase(0, 1);
    }
    if (normalizedPath.empty())
        return false;

    uint32_t count = 0;
    for (size_t i = 0; i < normalizedPath.length(); i++)
    {
        if (normalizedPath[i] == '~')
            count++;
    }
    if (count > 1)
        return false;
    else if (count == 1)
    {
        char const *home    = getenv("HOME");
        string      homeDir = string(home);
        pos                 = normalizedPath.find('~');
        normalizedPath.replace(pos, 1, homeDir);
    }
    return true;
}

string JrmeConfig::getPluginDir()
{
    return getConfigRootDir() + string("plugin/");
}

bool JrmeConfig::installIfNeed()
{
    bool   ret;
    string path   = getConfigRootDir();
    string target = path;

    if (access(path.c_str(), F_OK) != 0)
    {
        if (mkdir(path.c_str(), 0700) != 0)
        {
            JLOGE("[E] can not mkdir config dir:%s\n", path.c_str());
            return false;
        }
    }

    /*plugin dir*/
    target = getPluginDir();
    if (access(target.c_str(), F_OK) != 0)
    {
        if (mkdir(target.c_str(), 0700) != 0)
        {
            JLOGE("[E] can not mkdir config dir:%s\n", target.c_str());
            return false;
        }
    }

    target = getConfigFilePath();
    if (access(target.c_str(), R_OK | W_OK) != 0)
    {
        string cmd = string("touch ") + target;
        int    ret = system(cmd.c_str());
        if (system(cmd.c_str()) != 0)
            return false;
    }

    INI::File config;
    if (!config.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("[E] cannot not load ini file!");
        return false;
    }

    list<string> journalBooksList = readJournalBooksCfg();
    if (journalBooksList.size() == 0)
    {
        string dafaultJournal = getInitJournalBookPath();
        string gotPath;
        string savePath;
        printf("Path of your journal file (leave blank for %s):", dafaultJournal.c_str());
        getline(cin, gotPath);
        if (!gotPath.empty() && !pathNormalize(gotPath, savePath))
            return false;
        else if (gotPath.empty())
            savePath = dafaultJournal;
        journalBooksList.push_front(savePath);
        if (!writeJournalBooksCfg(journalBooksList))
            return false;
    }
    config.Save(target);
    return true;
}

string JrmeConfig::getEditorName()
{
    INI::File configFile;

    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("[E] cannot not load ini file!");
        return string();
    }

    return configFile.GetSection("base")->GetValue("editor", "vi").AsString();
}

list<string> JrmeConfig::getConfigNodePluginNames()
{
    INI::File configFile;

    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("[E] cannot not load ini file!");
        return list<string>();
    }

    string plugNames = configFile.GetSection("plugin")->GetValue("JAttribute").AsString();
    JLOGD("[D] JAttribute plugin ini:%s", plugNames.c_str());
    istringstream plugNameStream = istringstream(plugNames);
    string        pluginName;
    list<string>  plugNameVector;
    while (getline(plugNameStream, pluginName, ','))
    {
        plugNameVector.push_back(pluginName);
    }
    return plugNameVector;
}

string JrmeConfig::getDefaultJournalBookPath()
{
    list<string> allPath = readJournalBooksCfg();
    if (allPath.size() == 0)
        return getInitJournalBookPath();

    return *allPath.begin();
}

void JrmeConfig::setDeafultJournalBookPath(string path)
{
    list<string> allPath = readJournalBooksCfg();
    allPath.push_front(path);

    for (list<string>::iterator it = allPath.begin(); it != allPath.end();)
    {
        if (*it == path && it != allPath.begin())
        {
            allPath.erase(it);
            break;
        }
        else
            it++;
    }
    writeJournalBooksCfg(allPath);
    return;
}

list<string> JrmeConfig::getJournalIOPluginNames()
{
    INI::File configFile;
    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("[E] cannot not load ini file!");
        return list<string>();
    }
    string        plugNames      = configFile.GetSection("plugin")->GetValue("Formate").AsString();
    istringstream plugNameStream = istringstream(plugNames);
    string        pluginName;
    list<string>  plugins;
    while (getline(plugNameStream, pluginName, ','))
    {
        if (pluginName.length() > 0)
            plugins.push_back(pluginName);
    }
    return plugins;
}

list<string> JrmeConfig::readJournalBooksCfg()
{
    string       listPath = getJournalBooksCfgPath();
    list<string> allPath;
    ifstream     bookPathCfg;
    bookPathCfg.open(listPath, ios::in);
    if (!bookPathCfg.is_open())
    {
        JLOGW("[W] cann't open journal books list config file(%s). error:%s", listPath.c_str(), strerror(errno));
        return allPath;
    }
    string bookPath;
    while (getline(bookPathCfg, bookPath))
    {
        if (bookPath.length() > 0)
            allPath.push_back(bookPath);
    }
    return allPath;
}

bool JrmeConfig::writeJournalBooksCfg(list<string> allPath)
{
    string   listPath = getJournalBooksCfgPath();
    ofstream bookPathCfg;
    bookPathCfg.open(listPath, ios::out);
    if (!bookPathCfg.is_open())
    {
        JLOGW("[W] cann't open journal books list config file(%s). error:%s", listPath.c_str(), strerror(errno));
        return false;
    }
    for (auto &it : allPath)
    {
        if (it.length() > 0)
            bookPathCfg << it << endl;
    }
    bookPathCfg.close();
    return true;
}
