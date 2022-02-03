#include "Journal.h"

Journal::Journal(const string &title, const string &config, const string &content)
{
    mTitle = title;
    mConfig = config;
    mContent = content;
}

void Journal::setTitle(const string &title)
{
    mTitle = title;
}

void Journal::setConfig(const string &config)
{
    mConfig = config;
}

void Journal::setContent(const string &content)
{
    mContent = content;
}

string Journal::getTitle()
{
    return mTitle;
}

string Journal::getConfig()
{
    return mConfig;
}

string Journal::getContent()
{
    return mContent;
}

void Journal::clear()
{
    mTitle = string("");
    mConfig = string("");
    mContent = string("");
}

string Journal::toString()
{
    string out;
    out.append("==========journal==========\n");
    out.append(mTitle);
    if (mTitle[mTitle.length()-1] != '\n')
        out.append("\n");
    
    out.append("==========config==========\n");
    out.append(mConfig);
    if (mConfig[mConfig.length()-1] != '\n')
        out.append("\n");
    
    out.append("==========content==========\n");
    out.append(mContent);
    if (mContent[mContent.length()-1] != '\n')
        out.append("\n");
    
    out.append("===========================\n");

    return out;
}

bool Journal::operator ==(Journal &anotherJournal)
{
    if (this->mTitle == anotherJournal.mTitle &&
        this->mConfig == anotherJournal.mConfig &&
        this->mContent == anotherJournal.mContent)
        return true;
    
    return false;
}