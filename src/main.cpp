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
    for (size_t i = 1; i < argc; i++)
    {
       if (inStr.length()>0)
            inStr.append(string(" ") + argv[i]);
        else
            inStr.append(argv[i]);  
    }

    int32_t dividePos;
    for (int32_t i = inStr.length()-1; i >=0  ; i--)
    {
        if (inStr[i]==':')
        {
            dividePos = i;
            break;
        }
    }
    
    if (dividePos<0)
    {
        writeJournal(journalBookPath, string(""), inStr);
    }
    else if (dividePos==0)
    {
        inStr.erase(0 ,1);
        writeJournal(journalBookPath, string(""), inStr);
    }
    else if (0==inStr.length()-1)
    {
        inStr.erase(inStr.length()-1, 1);
        writeJournal(journalBookPath, inStr, string(""));
    }
    else
    {
        string timeStr = inStr.substr(0, dividePos);
        string titleStr = inStr.substr(dividePos+1, inStr.length() - dividePos - 1);
        writeJournal(journalBookPath, timeStr, titleStr);   
    }
    
    return 0;
}