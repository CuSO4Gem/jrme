#include "JrmeInstall.h"

#include "iniparser.hpp"

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

using namespace std;

void removeMultipleSpaces(string &str){
    uint32_t i=0;
    uint32_t gotSpace=0;
    while (i<str.length())
    {
        if (str[i]==' ')
            gotSpace++;
        else
            gotSpace=0;
        
        if (gotSpace>1)
        {
            str.erase(i,1);
        }
        else
        {
            i++;
        }
    }
}

string getConfigDir()
{
    char const* home = getenv("HOME");
    string homeDir = string(home)+string("/.config/");
    return homeDir;
}

/**
 * @brief 
 * 
 * @param pathIn 
 * @param normalizedPath 
 * @return true normalized success
 * @return false normalize fail
 */
bool pathNormalize(string &rawPath, string &normalizedPath)
{
    if (rawPath.empty())
        return false;

    normalizedPath = rawPath;
    size_t pos;

    while ((pos = normalizedPath.find('\t'))!=string::npos)
    {
        normalizedPath.replace(pos, 1, " ");
    }
    removeMultipleSpaces(normalizedPath);
    if (normalizedPath[0] == ' ')
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

string getDefaultJournalPath()
{
    char const* home = getenv("HOME");
    string homeDir = string(home)+string("/");
    return homeDir + string(".local/share/jrnl/journal.txt");
}

bool installIfNeed()
{
    bool ret;
    string path = getConfigDir();
    string target = path;

    if (access(path.c_str(), F_OK) != 0)
    {
        printf("error: can not access config file:%s\n", path.c_str());
        return false;
    }

    path += string("jrme/");
    if (access(path.c_str(), F_OK) != 0)
    {
        if(mkdir(path.c_str(), 0700) != 0)
        {
            printf("error: can not mkdir config dir:%s\n", path.c_str());
            return false;
        }
        
    }
    
    target = path+string("config.ini");
    if (access(target.c_str(), R_OK|W_OK) != 0)
    {
        FILE *fp = NULL;
        fp = fopen(target.c_str(), "w+");
        if (!fp)
        {
            printf("error: conn't create config file:%s\n", target.c_str());
            return false;
        }
        fclose(fp);
    }

    INI::File config = INI::File(target);
    string jrounBook = config.GetSection("journal books")->GetValue("default").AsString();
    if (jrounBook.empty())
    {
        string dafaultJournal = getDefaultJournalPath();
        string gotPath;
        string savePath;
        printf("Path to your journal file (leave blank for %s):", dafaultJournal.c_str());
        getline(cin, gotPath);
        if (!pathNormalize(gotPath, savePath))
            return false;

        config.GetSection("journal books")->SetValue("default", savePath);
        printf("jrounBook save to %s\n", savePath.c_str());
    }
    config.Save(target);
    return true;
}
