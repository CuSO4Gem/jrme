#include "TxtJournalIO.h"

#include <regex>
#include <unistd.h>

TxtJournalIO::TxtJournalIO()
{
    mState = UNINITED;
}

TxtJournalIO::~TxtJournalIO()
{
    if (mJrounal.is_open())
        mJrounal.close();
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

bool TxtJournalIO::setReadMod()
{
    if(mState==UNINITED)
        return false;

    if (mJrounal.is_open())
        mJrounal.close();

    mJrounal.open(mJournalPath, ios::in);
    if (!mJrounal)
        return false;

    mState = READ;
    return true;
}

bool TxtJournalIO::setWriteMode()
{
    if(mState==UNINITED)
        return false;

    if (mJrounal.is_open())
        mJrounal.close();

    mJrounal.open(mJournalPath, ios::out);
    if (!mJrounal)
        return false;

    mState = WRITE;
    return true;
}

bool TxtJournalIO::openJournal(string path)
{
    mJournalPath = path;
    mJrounal.open(mJournalPath, ios::in);

    if (!mJrounal)
    {
        /*maybe file no exist, try too create one*/
        mJrounal.open(mJournalPath, ios::out);
        if (!mJrounal)
            return false;
        else
        {
            mJrounal.close();
            return true;
        }
    }

    mJrounal.seekg(0, ios::end);
    if (mJrounal.tellg()<21)
    {
        mJrounal.close();
        return true;
    }
    mJrounal.seekg(0, ios::beg);

    /*check there have at list one journal in file*/
    string lineBuffer;
    smatch regexResult;
    bool finded;
    mJournalStart = 0;
    finded = false;
    //todo: more strong, some journal may leak some element
    while (getline(mJrounal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            mJournalStart = mJrounal.tellg();
    }
    if (!finded)
    {
        mJrounal.close();
        return false;
    }

    finded = false;
    while (getline(mJrounal, lineBuffer))
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
        mJrounal.close();
        return false;
    }

    finded = false;
    while (getline(mJrounal, lineBuffer))
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
        mJrounal.close();
        return false;
    }

    //note need to find end ======

    mJrounal.close();
    mState = INITED;
    return true;
}

shared_ptr<Journal> TxtJournalIO::readJournal()
{
    if (!mJrounal.is_open() || mState!=READ)
        return shared_ptr<Journal>(nullptr);
    
    if (mJrounal.tellg() == 0)
        mJrounal.seekg(mJournalStart);
    
    shared_ptr<Journal> journl = shared_ptr<Journal>(new Journal);
    string lineBuffer;
    string readBuffer;
    smatch regexResult;
    bool finded;
    finded = false;
    //todo: more strong
    while (getline(mJrounal, lineBuffer))
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
        mJrounal.close();
        mState = INITED;
        return shared_ptr<Journal>(nullptr);
    }

    finded = false;
    readBuffer.clear();
    while (getline(mJrounal, lineBuffer))
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
        mJrounal.close();
        mState = INITED;
        return shared_ptr<Journal>(nullptr);
    }
    journl->setTitle(readBuffer);

    finded = false;
    readBuffer.clear();
    while (getline(mJrounal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mJrounal.close();
        mState = INITED;
        return shared_ptr<Journal>(nullptr);
    }
    journl->setConfig(readBuffer);

    finded = false;
    readBuffer.clear();
    while (getline(mJrounal, lineBuffer))
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
        mJrounal.close();
        mState = INITED;
    }
    journl->setContent(readBuffer);

    return journl;
}

bool TxtJournalIO::writeJournal(shared_ptr<Journal> journal)
{
    if (!mJrounal.is_open() || mState!=WRITE)
        return false;

    string lineBuffer = string("==========journal==========\n");
    string title = journal->getTitle();
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());
    mJrounal.write(title.c_str(), title.length());
    if (title[title.length()-1]!='\n')
        mJrounal.write("\n", 1);

    string config = journal->getConfig();
    lineBuffer = string("==========config==========\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());
    mJrounal.write(config.c_str(), config.length());
    if (config[config.length()-1]!='\n')
        mJrounal.write("\n", 1);

    string content = journal->getContent();
    lineBuffer = string("==========content==========\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());
    mJrounal.write(content.c_str(), content.length());
    if (content[content.length()-1]!='\n')
        mJrounal.write("\n", 1);

    lineBuffer = string("===========================\n\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());

    return true;
}
