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
#include <regex>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

#include "debug_print.h"
#include "SfJournalBook.h"
#include "Utils.h"

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
            *pos += 1;
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

bool attributePartGetLine(istringstream &attributePartStream, string &key, string &value)
{
    string lineBuffer;
    key.clear();
    value.clear();
    if(!getline(attributePartStream, lineBuffer))
        return false;
    
    value = lineBuffer;
    if (value.length() == 0)
        return true;
    
    tabToSpace(value);
    size_t pos = 0;
    while (pos<value.length())
    {
        if (value[pos]=='=')
        {
            key = value.substr(0, pos);
            break;
        }
        pos++;
    }

    value.erase(0, min(pos+1, value.length()));
    
    /*remove space front and back*/
    while (key.length()>0 && key[0]==' ')
    {
        key.erase(0);
    }
    while (key.length()>0 && key[key.length()-1]==' ')
    {
        key.erase(key.length()-1);
    }

    if (value.length()==0)
        return true;
    
    /*remove space front and back*/
    while (value.length()>0 && value[0]==' ')
    {
        value.erase(0, 1);
    }
    while (value.length()>0 && value[value.length()-1]==' ')
    {
        value.erase(value.length()-1, 1);
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

bool getValueFromJAttributePart(const string &attributePart, const string &key, string &value)
{
    string keyBuffer, valueBuffer;
    istringstream attributePartStream = istringstream(attributePart);
    
    bool finded = false;
    while(attributePartGetLine(attributePartStream, keyBuffer, valueBuffer))
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

bool setValueToJAttributePart(string &attributePart, const string &key,const string value)
{
    string keyBuffer, valueBuffer, lineBuffer;
    istringstream attributePartStream = istringstream(attributePart);
    
    bool finded = false;
    size_t keyBeginPos = attributePartStream.tellg();
    while(attributePartGetLine(attributePartStream, keyBuffer, valueBuffer))
    {
        if (keyBuffer==key)
        {
            finded = true;
            break;
        }
        keyBeginPos = attributePartStream.tellg();
    }
    if (!finded)
        return false;
    
    attributePartStream.seekg(keyBeginPos);
    getline(attributePartStream, lineBuffer);
    size_t len = lineBuffer.length();
    attributePart.erase(keyBeginPos, len);
    string insertString = key+"="+value;
    attributePart.insert(keyBeginPos, insertString);

    return true;
}

time_t getStampFormJAttributePart(const string &attributePart)
{
    string key, value;
    istringstream attributePartStream = istringstream(attributePart);
    
    bool finded = false;
    while(attributePartGetLine(attributePartStream, key, value))
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
            if (hour<0 || hour>24)
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

int32_t getLevelFormJAttributePart(const string &attributePart)
{
    string key, value;
    istringstream attributePartStream = istringstream(attributePart);
    
    bool finded = false;
    while(attributePartGetLine(attributePartStream, key, value))
    {
        if (key==string("level"))
        {
            finded = true;
            break;
        }
    }

    if (!finded || value.length()==0)
    {
        return 0;
    }

    size_t pos;
    for (pos = 0; pos < value.length(); pos++)
    {
        if (isNumChar(value[pos]) || value[pos]=='-')
            break;
    }
    if (pos == value.length())
        return 0;
    
    return atoi(value.c_str()+pos);
}

vector<string> getTagsFormJAttributePart(const string &attributePart)
{
    string key, value;
    istringstream attributePartStream = istringstream(attributePart);
    
    bool finded = false;
    while(attributePartGetLine(attributePartStream, key, value))
    {
        if (key==string("tags"))
        {
            finded = true;
            break;
        }
    }
    
    if (!finded || value.length()==0)
    {
        return vector<string>();
    }

    size_t tagsSize=0;
    for (size_t i = 0; i < value.length(); i++)
    {
        if (value[i] == ';')
        {
            tagsSize++;
        }
    }

    vector<string> tags = vector<string>();
    tags.reserve(tagsSize+1);
    istringstream tagsStream = istringstream(value);
    string tag;
    while (getline(tagsStream, tag, ';'))
    {
        // remove space at front
        size_t i=0;
        while (i<tag.length() && tag[i]==' ')
        {
            i++;
        }
        if (i==tag.length())
            continue;
        else
            tag.erase(0, i);

        tags.push_back(tag);
    }
    return tags;
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
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}attributePart[ ]{0,}={2,}")))
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
    journl->setAttributePart(readBuffer);
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

string validPath(string rawPath)
{
    if (rawPath.length()==0)
        return rawPath;
    while (rawPath[0] == ' ')
    {
        rawPath.erase(0, 1);
    }
    if (rawPath.length()==0)
        return rawPath;
    
    uint32_t count=0;
    for (size_t i = 0; i < rawPath.length(); i++)
    {
        if (rawPath[i] == '~')
            count++;
    }
    if (count>1)
        return string("");
    else if (count==1)
    {
        char const* home = getenv("HOME");
        string homeDir = string(home);
        size_t pos;
        pos = rawPath.find('~');
        rawPath.replace(pos, 1, homeDir);
    }
    return rawPath;
}

string getFilePath()
{
    string inputPath;
    getline(cin, inputPath);
    
    return validPath(inputPath);
}

shared_ptr<JournalBookBase> bookFactory(string path)
{
    struct stat sBuf;
    stat(path.c_str(), &sBuf);
    if (S_ISDIR(sBuf.st_mode))
    {
        return nullptr;
    }
    else
        return make_shared<SfJournalBook>();
}