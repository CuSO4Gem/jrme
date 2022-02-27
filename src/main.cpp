#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <string>

using namespace std;

#include "WriteMode.h"
#include "JrmeConfig.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
    if (!installIfNeed())
    {
        printf("error: install jrme fail!!\n");
        return -1;
    }
    
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

    int32_t dividePos = -1;
    for (size_t i = 0; i < inStr.length(); i++)
    {
        if (inStr[i]==':')
        {
            if (i==0 || i == inStr.length()-1 || 
            !(isNumChar(inStr[i-1]) && isNumChar(inStr[i+1])))
            {
                dividePos = i;
            }
        }
    }
    
    if (dividePos<0)
    {
        journlWriteMode(journalBookPath, string(""), inStr);
    }
    else if (dividePos==0)
    {
        inStr.erase(0 ,1);
        journlWriteMode(journalBookPath, string(""), inStr);
    }
    else
    {
        string timeStr = inStr.substr(0, dividePos);
        string titleStr = inStr.substr(dividePos+1, inStr.length() - dividePos - 1);
        journlWriteMode(journalBookPath, timeStr, titleStr);   
    }
    
    return 0;
}