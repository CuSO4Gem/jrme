#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <string>

using namespace std;

#include "WriteMode.h"
#include "JrmeConfig.h"

int main(int argc, char* argv[]) {
    bool ret;
    ret = installIfNeed();
    if (ret=false)
        printf("error: install jrme fail!!\n");
    
    INI::File configFile = INI::File(getConfigFilePath());
    string journalBookPath = configFile.GetSection("journal books")->GetValue("default").AsString();
    string inStr;
    uint32_t part = 0;
    for (size_t i = 1; i < argc; i++)
    {
       if (inStr.length()>0)
            inStr.append(string(" ") + argv[i]);
        else
            inStr.append(argv[i]);  
    }

    if (inStr.find(":")==string::npos)
    {
        writeJournal(journalBookPath, string(""), inStr);
    }
    else if (inStr.find(":")==0)
    {
        inStr.erase(0 ,1);
        writeJournal(journalBookPath, string(""), inStr);
    }
    else if (inStr.find(":")==inStr.length()-1)
    {
        inStr.erase(inStr.length()-1, 1);
        writeJournal(journalBookPath, inStr, string(""));
    }
    else
    {
        string timeStr = inStr.substr(0, inStr.find(":"));
        string titleStr = inStr.substr(inStr.find(":")+1, inStr.length() - inStr.find(":") - 1);
        // printf("timeStr:%s\ntitleStr:%s", timeStr.c_str(), titleStr.c_str());
        writeJournal(journalBookPath, timeStr, titleStr);
    }
    
    return 0;
}