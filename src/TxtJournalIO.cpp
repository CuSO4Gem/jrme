#include "TxtJournalIO.h"

#include <regex>
#include <unistd.h>

TxtJournalIO::TxtJournalIO()
{
    mState = UNINITED;
}

TxtJournalIO::~TxtJournalIO()
{
    if (mjournal.is_open())
        mjournal.close();
}

uint32_t TxtJournalIO::apiSupport()
{
    return 1;
}

list<string> TxtJournalIO::formateSupport()
{
    list<string> formates {string("txt")};
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

    if (mjournal.is_open())
        mjournal.close();

    mjournal.open(mJournalPath, ios::in);
    if (!mjournal)
        return false;

    mState = READ;
    return true;
}

bool TxtJournalIO::setWriteMode()
{
    if(mState==UNINITED)
        return false;

    if (mjournal.is_open())
        mjournal.close();

    mjournal.open(mJournalPath, ios::out);
    if (!mjournal)
        return false;

    mState = WRITE;
    return true;
}

bool TxtJournalIO::open(string path)
{
    mJournalPath = path;
    mjournal.open(mJournalPath, ios::in);

    if (!mjournal)
    {
        /*maybe file no exist, try too create one*/
        mjournal.open(mJournalPath, ios::out);
        if (!mjournal)
            return false;
        else
        {
            mjournal.close();
            mState = INITED;
            return true;
        }
    }

    /*an empty file, just rewrite it*/
    mjournal.seekg(0, ios::end);
    if (0==mjournal.tellg())
    {
        mjournal.close();
        mState = INITED;
        return true;
    }
    mjournal.seekg(0, ios::beg);

    /*check there have at list one journal in file*/
    string lineBuffer;
    smatch regexResult;
    bool finded;
    mJournalStart = 0;
    finded = false;
    //todo: more strong, some journal may leak some element
    while (getline(mjournal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            mJournalStart = mjournal.tellg();
    }
    if (!finded)
    {
        mjournal.close();
        return false;
    }

    finded = false;
    while (getline(mjournal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}config[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        mjournal.close();
        return false;
    }

    finded = false;
    while (getline(mjournal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
    }
    if (!finded)
    {
        mjournal.close();
        return false;
    }

    //note need to find end ======

    mjournal.close();
    mState = INITED;
    return true;
}

shared_ptr<Journal> TxtJournalIO::readJournal()
{
    if (!mjournal.is_open() || mState!=READ)
        return nullptr;
    
    if (mjournal.tellg() == 0)
        mjournal.seekg(mJournalStart);
    
    shared_ptr<Journal> journl = shared_ptr<Journal>(new Journal);
    string lineBuffer;
    string readBuffer;
    smatch regexResult;
    bool finded;
    finded = false;
    //todo: more strong
    while (getline(mjournal, lineBuffer))
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
        mjournal.close();
        mState = INITED;
        return nullptr;
    }

    finded = false;
    readBuffer.clear();
    while (getline(mjournal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}config[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mjournal.close();
        mState = INITED;
        return nullptr;
    }
    journl->setTitle(readBuffer);

    finded = false;
    readBuffer.clear();
    while (getline(mjournal, lineBuffer))
    {
        size_t prLen=0;
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
        mjournal.close();
        mState = INITED;
        return nullptr;
    }
    journl->setConfig(readBuffer);

    finded = false;
    readBuffer.clear();
    while (getline(mjournal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={4,}[ ,\t,\r,\n]{0,}$")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mjournal.close();
        mState = INITED;
    }
    journl->setContent(readBuffer);

    return journl;
}

bool TxtJournalIO::writeJournal(shared_ptr<Journal> journal)
{
    if (!mjournal.is_open() || mState!=WRITE)
        return false;

    string lineBuffer = string("==========journal==========\n");
    string title = journal->getTitle();
    mjournal.write(lineBuffer.c_str(), lineBuffer.length());
    mjournal.write(title.c_str(), title.length());
    if (title[title.length()-1]!='\n')
        mjournal.write("\n", 1);

    string config = journal->getConfig();
    lineBuffer = string("==========config==========\n");
    mjournal.write(lineBuffer.c_str(), lineBuffer.length());
    mjournal.write(config.c_str(), config.length());
    if (config[config.length()-1]!='\n')
        mjournal.write("\n", 1);

    string content = journal->getContent();
    lineBuffer = string("==========content==========\n");
    mjournal.write(lineBuffer.c_str(), lineBuffer.length());
    mjournal.write(content.c_str(), content.length());
    if (content[content.length()-1]!='\n')
        mjournal.write("\n", 1);

    lineBuffer = string("===========================\n\n");
    mjournal.write(lineBuffer.c_str(), lineBuffer.length());

    return true;
}
