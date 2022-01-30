#include <iostream>
#include <list>
#include <regex>
#include <sstream>

#include "Utils.h"

inline bool isNumChar(const char ch)
{
    return '0'<=ch && ch<='9';
}

bool seekToNumChar(size_t *pos, const string &inStr)
{
    if (*pos<0 || *pos>=inStr.length())
        return false;
    bool finded = false;
    while (true)
    {
        if (*pos>inStr.length()-1)
            break;
        else if (isNumChar(inStr[*pos]))
        {
            finded = true;
            break;
        }
        else
            *pos++;
    }

    return finded;
}

void jumpNumChar(size_t *pos, const string &inStr)
{
    while (true)
    {
        if (*pos>=inStr.length() || !isNumChar(inStr[*pos]))
            break;
        else
            *pos += 1;
    }
}

void tabToSpace(string &str)
{
    size_t pos;
    while (pos<str.length())
    {
        if (str[pos] == '\t')
            str[pos] = ' ';
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

bool getValueFromConfig(const string &config, const string &key, string &value)
{
    string keyBuffer, valueBuffer;
    istringstream configStream = istringstream(config);
    
    bool finded = false;
    while(configGetline(configStream, keyBuffer, valueBuffer))
    {
        if (keyBuffer==key)
        {
            value = valueBuffer;
            finded = true;
            break;
        }
    }
    return finded;
}

bool setValueToConfig(string &config, const string &key,const string value)
{
    string keyBuffer, valueBuffer, lineBuffer;
    istringstream configStream = istringstream(config);
    
    bool finded = false;
    size_t keyBeginPos = configStream.tellg();
    while(configGetline(configStream, keyBuffer, valueBuffer))
    {
        if (keyBuffer==key)
        {
            finded = true;
            break;
        }
        keyBeginPos = configStream.tellg();
    }
    if (!finded)
        return false;
    
    configStream.seekg(keyBeginPos);
    getline(configStream, lineBuffer);
    size_t len = lineBuffer.length();
    config.erase(keyBeginPos, len);
    string insertString = key+"="+value;
    config.insert(keyBeginPos, insertString);

    return true;
}

time_t getStampFormConfig(const string &config)
{
    string key, value;
    istringstream configStream = istringstream(config);
    
    bool finded = false;
    while(configGetline(configStream, key, value))
    {
        if (key==string("date"))
        {
            finded = true;
            break;
        }
    }

    if (!finded)
    {
        return 0;
    }

    size_t pos=0;
    list<int> numberlist;
    while (true)
    {
        if (seekToNumChar(&pos, value))
        {
            numberlist.push_back(atoi(&value[pos]));
            jumpNumChar(&pos, value);
        }
        else
            break;
    }
    
    if (numberlist.size()==0)
        return 0;
    
    Date date = Date(1970, 01, 01, 9, 00, 00);
    uint32_t numberOder=0;
    for (auto &it:numberlist)
    {
        int year, month, day, hour, minute, second;
        switch (numberOder)
        {
        case 0:
            year = it;
            if (year<1970 || year>9999)
                return 0;
            else
                date.setYear(year);
            break;
        
        case 1:
            month = it;
            if (month<1 || month>12)
                return date.stamp();
            else
                date.setMonth(month);
            break;
        
        case 2:
            day = it;
            if (day<1 || day>31)
                return date.stamp();
            else
                date.setDay(day);
            break;
        
        case 3:
            hour = it;
            if (hour<0 || hour>12)
                return date.stamp();
            else
                date.setHour(hour);
            break;
        
        case 4:
            minute = it;
            if (minute<0 || minute>60)
                return date.stamp();
            else
                date.setMinute(minute);
            break;
        
        case 5:
            second = it;
            if (second<0 || second>60)
                return date.stamp();
            else
                date.setSecond(second);
            break;

        default:
            break;
        }
        numberOder++;
        if (numberOder>5)
            break;
    }
    
    return date.stamp();
}

shared_ptr<Journal> strToJournal(const string &inStr)
{
    istringstream strStream = istringstream(inStr);
    shared_ptr<Journal> journl = make_shared<Journal>();
    string readBuffer;
    string lineBuffer;
    smatch regexResult;
    bool finded;
    finded = false;

    while (getline(strStream, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        return nullptr;
    }

    readBuffer.clear();
    while (getline(strStream, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}config[ ]{0,}={2,}")))
        {
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    journl->setTitle(readBuffer);
    if (strStream.eof())
        return journl;


    readBuffer.clear();
    while (getline(strStream, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            break;
        }
        
        /* ignore the line wich without any content*/
        bool haveContent = false;
        for (size_t i = 0; i < lineBuffer.length(); i++)
        {
            if (lineBuffer[i] != ' ' &&
                lineBuffer[i] != '\t' &&
                lineBuffer[i] != '\r' &&
                lineBuffer[i] != '\n')
            {
                haveContent = true;
                break;
            }
        }
        
        if (haveContent)
            readBuffer.append(lineBuffer + string("\n"));
    }
    journl->setConfig(readBuffer);
    if (strStream.eof())
        return journl;

    readBuffer.clear();
    while (getline(strStream, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^#{4,}[ ,\t,\r,\n]{0,}$")))
        {
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    journl->setContent(readBuffer);

    return journl;
}