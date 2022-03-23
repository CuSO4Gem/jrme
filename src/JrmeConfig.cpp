
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "JrmeConfig.h"
#include "debug_print.h"
#include "Utils.h"

using namespace std;


string JrmeConfig::getConfigRootDir()
{
    char const* home = getenv("HOME");
    string dir = string(home)+string("/.jrme/");
    return dir;
}

string JrmeConfig::getInitJournalBookPath()
{
    return JrmeConfig::getConfigRootDir() + string("journal.txt");
}

string JrmeConfig::journalBooksPathCfg()
{
    return getConfigRootDir() + string("booklist.txt");
}

string JrmeConfig::getConfigFilePath()
{
    return getConfigRootDir()+string("config.ini");
}

/**
 * @brief '\t' to space, '~' to dir
 * 
 * @param pathIn 
 * @param normalizedPath 
 * @return true normalized success
 * @return false normalize fail
 */
bool JrmeConfig::pathNormalize(string &rawPath, string &normalizedPath)
{
    if (rawPath.empty())
        return false;

    normalizedPath = rawPath;
    size_t pos;

    while ((pos = normalizedPath.find('\t'))!=string::npos)
    {
        normalizedPath.replace(pos, 1, " ");
    }
    while (normalizedPath[0] == ' ')
    {
        normalizedPath.erase(0, 1);
    }
    if (normalizedPath.empty())
        return false;
    
    uint32_t count=0;
    for (size_t i = 0; i < normalizedPath.length(); i++)
    {
        if (normalizedPath[i] == '~')
            count++;
    }
    if (count>1)
        return false;
    else if (count==1)
    {
        char const* home = getenv("HOME");
        string homeDir = string(home);
        pos = normalizedPath.find('~');
        normalizedPath.replace(pos, 1, homeDir);
    }
    return true;
}


string JrmeConfig::getPluginDir()
{
    return getConfigRootDir()+string("plugin/");
}

bool JrmeConfig::installIfNeed()
{
    bool ret;
    string path = getConfigRootDir();
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
    if (access(target.c_str(), R_OK|W_OK) != 0)
    {
        string cmd = string("touch ") + target;
        int ret = system(cmd.c_str());
        if (system(cmd.c_str()) != 0)
            return false;
    }
    
    INI::File config;
    if (!config.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("cannot not load ini file!");
        return false;
    }

    string journBook = config.GetSection("journal books")->GetValue("default").AsString();
    if (journBook.empty())
    {
        string dafaultJournal = getInitJournalBookPath();
        string gotPath;
        string savePath;
        printf("Path of your journal file (leave blank for %s):", dafaultJournal.c_str());
        getline(cin, gotPath);
        if (!gotPath.empty() && !pathNormalize(gotPath, savePath))
            return false;
        else if(gotPath.empty())
            savePath = dafaultJournal;
        config.GetSection("journal books")->SetValue("default", savePath);
        printf("journBook save to %s\n", savePath.c_str());
    }
    config.Save(target);
    return true;
}

string JrmeConfig::getEditorName()
{
    INI::File configFile;
    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("cannot not load ini file!");
        return string();
    }
        
    return configFile.GetSection("base")->GetValue("editor", "vi").AsString();
}

vector<string> JrmeConfig::getConfigNodePluginNames()
{
    INI::File configFile;
    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("cannot not load ini file!");
        return vector<string>();
    }

    string plugNames = configFile.GetSection("plugin")->GetValue("config node").AsString();
    istringstream plugNameStream = istringstream(plugNames);
    string pluginName;
    vector<string> plugNameVector;
    while (getline(plugNameStream, pluginName, ','))
    {
        plugNameVector.push_back(pluginName);
    }
    return plugNameVector;
}

string JrmeConfig::getDefaultJournalBookPath()
{
    INI::File configFile;
    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("cannot not load ini file!");
        return string();
    }
    
    return configFile.GetSection("journal books")->GetValue("default", getInitJournalBookPath).AsString();
}

void JrmeConfig::setDeafultJournalBookPath(string path)
{
    if (path.length()==0)
        return;

    INI::File configFile;
    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("cannot not load ini file!");
        return ;
    }
    
    configFile.GetSection("journal books")->SetValue("default", path);
}


vector<string> JrmeConfig::getJournalIOPluginNames()
{
    INI::File configFile;
    if (!configFile.Load(JrmeConfig::getConfigFilePath()))
    {
        JLOGE("cannot not load ini file!");
        return vector<string> ();
    }
    string plugNames = configFile.GetSection("plugin")->GetValue("journal format").AsString();
    istringstream plugNameStream = istringstream(plugNames);
    string pluginName;
    vector<string> plugins;
    while (getline(plugNameStream, pluginName, ','))
    {
        if (pluginName.length()>0)
            plugins.push_back(pluginName);
    }
    return plugins;
}

vector<string> JrmeConfig::getJournalBooksPath()
{
    string listPath = journalBooksPathCfg();
    vector<string> allPath;
    ifstream bookPathCfg;
    bookPathCfg.open(listPath, ios::in);
    if (!bookPathCfg.is_open())
    {
        JLOGW("[W] cann't open journal books list config file(%s). error:%s", listPath.c_str(), strerror(errno));
        return allPath;
    }
    string bookPath;
    while (getline(bookPathCfg, bookPath))
    {
        allPath.push_back(bookPath);
    }
    return allPath;
}

void JrmeConfig::saveJournalBooksPath(vector<string> allPath)
{
    string listPath = journalBooksPathCfg();
    ofstream bookPathCfg;
    bookPathCfg.open(listPath, ios::out);
    if (!bookPathCfg.is_open())
    {
        JLOGW("[W] cann't open journal books list config file(%s). error:%s", listPath.c_str(), strerror(errno));
        return;
    }
    for (auto &it:allPath)
    {
        bookPathCfg << it << endl;
    }
    bookPathCfg.close();
}
