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

bool TxtJournalIO::readJournal(string &title, string &config, string &content)
{
    if (!mJrounal.is_open() || mState!=READ)
        return false;
    
    if (mJrounal.tellg() == 0)
        mJrounal.seekg(mJournalStart);
    
    string lineBuffer;
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
        return false;
    }

    finded = false;
    title.clear();
    while (getline(mJrounal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}config[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            title.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mJrounal.close();
        mState = INITED;
        return false;
    }

    finded = false;
    config.clear();
    while (getline(mJrounal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            config.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mJrounal.close();
        mState = INITED;
        return false;
    }

    finded = false;
    content.clear();
    while (getline(mJrounal, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={4,}[ ,\t,\r,\n]{0,}$")))
        {
            finded = true;
            break;
        }
        else
            content.append(lineBuffer + string("\n"));
    }
    if (!finded)
    {
        mJrounal.close();
        mState = INITED;
    }

    return true;
}

bool TxtJournalIO::writeJournal(const string &title, const string &config, const string &content)
{
    if (!mJrounal.is_open() || mState!=WRITE)
        return false;

    string lineBuffer = string("==========journal==========\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());
    mJrounal.write(title.c_str(), title.length());
    if (title[title.length()-1]!='\n')
        mJrounal.write("\n", 1);

    lineBuffer = string("==========config==========\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());
    mJrounal.write(config.c_str(), config.length());
    if (config[config.length()-1]!='\n')
        mJrounal.write("\n", 1);

    lineBuffer = string("==========content==========\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());
    mJrounal.write(content.c_str(), content.length());
    if (content[content.length()-1]!='\n')
        mJrounal.write("\n", 1);

    lineBuffer = string("===========================\n\n");
    mJrounal.write(lineBuffer.c_str(), lineBuffer.length());

    return true;
}
