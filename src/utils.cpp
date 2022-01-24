#include <iostream>
#include <sstream>

#include "utils.h"

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

void tabToSpace(string &str)
{
    size_t pos;
    while (pos<str.length())
    {
        if (str[pos] == '\t')
        pos++;
    }
}

bool configGetline(istringstream &configStream, string &key, string &value)
{
    string lineBuffer;
    key.clear();
    value.clear();
    if(!getline(configStream, lineBuffer))
        return false;
    
    if (lineBuffer.length() == 0)
        return true;
    
    tabToSpace(lineBuffer);
    size_t pos = 0;
    while (pos<lineBuffer.length())
    {
        if (lineBuffer[pos]=='=')
        {
            key = lineBuffer.substr(0, pos);
            break;
        }
        pos++;
    }

    lineBuffer.erase(0, min(pos+1, lineBuffer.length()));
    
    /*remove space front and back*/
    while (key.length()>0 && key[0]==' ')
    {
        key.erase(0);
    }
    while (key.length()>0 && key[key.length()-1]==' ')
    {
        key.erase(key.length()-1);
    }

    if (lineBuffer.length()==0)
        return true;
    
    value = lineBuffer;
    /*remove space front and back*/
    while (value.length()>0 && value[0]==' ')
    {
        value.erase(0);
    }
    while (value.length()>0 && value[value.length()-1]==' ')
    {
        value.erase(value.length()-1);
    }
    return true;
}

Date getJournalFormConfig(string &config)
{
    string lineBuffer;
    istringstream configStream = istringstream(config);
    
    while(getline(configStream, lineBuffer))
    {

    }
}