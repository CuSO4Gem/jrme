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
#include "TxtJournalIO.h"

#include <regex>
#include <unistd.h>

/*debug */
#include "debug_print.h"

TxtJournalIO::TxtJournalIO()
{
    mState = UNINITED;
}

TxtJournalIO::~TxtJournalIO()
{
    if (mJournal.is_open())
        mJournal.close();
}

uint32_t TxtJournalIO::apiSupport()
{
    return 1;
}

vector<string> TxtJournalIO::formateSupport()
{
    vector<string> formates {string("txt")};
    return formates;
}

bool TxtJournalIO::isSupportAes256()
{
    return false;
}

void TxtJournalIO::setKey(uint8_t key[32])
{
    return;
}

void TxtJournalIO::clearKey()
{
    return;
}

bool TxtJournalIO::setReadMod()
{
    if(mState==UNINITED)
        return false;

    if (mJournal.is_open())
        mJournal.close();

    mJournal.open(mJournalPath, ios::in);
    if (!mJournal)
        return false;

    mState = READ;
    return true;
}

bool TxtJournalIO::setWriteMode()
{
    if(mState==UNINITED)
        return false;

    if (mJournal.is_open())
        mJournal.close();

    mJournal.open(mJournalPath, ios::out);
    if (!mJournal)
        return false;

    mState = WRITE;
    return true;
}

bool TxtJournalIO::open(string path)
{
    mJournalPath = path;
    mJournal.open(mJournalPath, ios::in);

    if (!mJournal)
    {
        /*maybe file no exist, try too create one*/
        string cmd = string("touch ")+path;
        JLOGI("[I] touch journal file: %s",cmd.c_str());
        int ret = system(cmd.c_str());
        if (ret!=0)
            return false;
        else
        {
            mState = INITED;
            return true;
        }
    }

    /*an empty file, just rewrite it*/
    mJournal.seekg(0, ios::end);
    if (0==mJournal.tellg())
    {
        mJournal.close();
        mState = INITED;
        return true;
    }
    mJournal.seekg(0, ios::beg);

    /*check there have at list one journal in file*/
    string lineBuffer;
    smatch regexResult;
    bool finded;
    finded = false;
    //todo: more strong, some journal may leak some element
    while (getline(mJournal, lineBuffer))
    {
        
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        mJournal.close();
        return false;
    }

    finded = false;
    while (getline(mJournal, lineBuffer))
    {
        
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}attributePart[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        mJournal.close();
        return false;
    }

    finded = false;
    while (getline(mJournal, lineBuffer))
    {
        
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        mJournal.close();
        return false;
    }

    //note need to find end ======

    mJournal.close();
    mState = INITED;
    return true;
}

void TxtJournalIO::close()
{
    mJournal.close();
    mState = UNINITED;
}

shared_ptr<Journal> TxtJournalIO::readJournal()
{
    if (!mJournal.is_open() || mState!=READ)
        return nullptr;
        
    shared_ptr<Journal> journl = make_shared<Journal>();
    string lineBuffer;
    string readBuffer;
    smatch regexResult;
    bool finded;
    finded = false;
    //todo: more strong
    //read title
    while (getline(mJournal, lineBuffer))
    {
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        mJournal.close();
        mState = INITED;
        return nullptr;
    }

    //read attributePart
    finded = false;
    readBuffer.clear();
    while (getline(mJournal, lineBuffer))
    {
        
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}attributePart[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mJournal.close();
        mState = INITED;
        return nullptr;
    }
    if (readBuffer[readBuffer.length()-1] != '\n')
        readBuffer.append("\n");
    journl->setTitle(readBuffer);

    //read content
    finded = false;
    readBuffer.clear();
    while (getline(mJournal, lineBuffer))
    {
        
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            finded = true;
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
    if (!finded)
    {
        mJournal.close();
        mState = INITED;
        return nullptr;
    }
    if (readBuffer[readBuffer.length()-1] != '\n')
        readBuffer.append("\n");
    journl->setAttributePart(readBuffer);

    finded = false;
    readBuffer.clear();
    while (getline(mJournal, lineBuffer))
    {
        
        if (regex_search(lineBuffer, regexResult, regex("^#{4,}[ ,\t,\r,\n]{0,}$")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mJournal.close();
        mState = INITED;
    }
    if (readBuffer[readBuffer.length()-1] != '\n')
        readBuffer.append("\n");
    journl->setContent(readBuffer);

    return journl;
}

bool TxtJournalIO::writeJournal(shared_ptr<Journal> journal)
{
    if (!mJournal.is_open() || mState!=WRITE)
        return false;

    string lineBuffer = string("==========journal==========\n");
    string title = journal->getTitle();
    mJournal.write(lineBuffer.c_str(), lineBuffer.length());
    mJournal.write(title.c_str(), title.length());
    if (title[title.length()-1]!='\n')
        mJournal.write("\n", 1);

    string attributePart = journal->getAttributePart();
    lineBuffer = string("=======attributePart=======\n");
    mJournal.write(lineBuffer.c_str(), lineBuffer.length());
    mJournal.write(attributePart.c_str(), attributePart.length());
    if (attributePart[attributePart.length()-1]!='\n')
        mJournal.write("\n", 1);

    string content = journal->getContent();
    lineBuffer = string("==========content==========\n");
    mJournal.write(lineBuffer.c_str(), lineBuffer.length());
    mJournal.write(content.c_str(), content.length());
    if (content[content.length()-1]!='\n')
        mJournal.write("\n", 1);

    lineBuffer = string("###########################\n\n");
    mJournal.write(lineBuffer.c_str(), lineBuffer.length());

    return true;
}
