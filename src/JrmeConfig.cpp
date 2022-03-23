
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "JrmeConfig.h"
#include "debug_print.h"
#include "Utils.h"

using namespace std;

string JrmeConfig::getDefaultJournalPath()
{
    return JrmeConfig::getConfigRootDir() + string("journal.txt");
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


string JrmeConfig::getConfigRootDir()
{
    char const* home = getenv("HOME");
    string dir = string(home)+string("/.jrme/");
    return dir;
}

string JrmeConfig::getConfigFilePath()
{
    return getConfigRootDir()+string("config.ini");
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
    
    INI::File config = INI::File(target);
    string journBook = config.GetSection("journal books")->GetValue("default").AsString();
    if (journBook.empty())
    {
        string dafaultJournal = getDefaultJournalPath();
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

vector<string> JrmeConfig::getJournalIOPluginNames()
{
    INI::File configFile = INI::File(getConfigFilePath());
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


