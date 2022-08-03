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
#include "Journal.h"

using std::hash;

Journal::Journal(const string &title, const string &attributePart, const string &content)
{
    mTitle         = title;
    mAttributePart = attributePart;
    mContent       = content;
}

void Journal::setTitle(const string &title)
{
    mTitle = title;
}

void Journal::setAttributePart(const string &attributePart)
{
    mAttributePart = attributePart;
}

void Journal::setContent(const string &content)
{
    mContent = content;
}

string Journal::getTitle()
{
    return mTitle;
}

string &Journal::getAttributePart()
{
    return mAttributePart;
}

string Journal::getContent()
{
    return mContent;
}

void Journal::clear()
{
    mTitle         = string("");
    mAttributePart = string("");
    mContent       = string("");
}

string Journal::toString()
{
    string out;
    out.append("==========journal==========\n");
    out.append(mTitle);
    if (mTitle[mTitle.length() - 1] != '\n')
        out.append("\n");

    out.append("=======attributePart=======\n");
    out.append(mAttributePart);
    if (mAttributePart[mAttributePart.length() - 1] != '\n')
        out.append("\n");

    out.append("==========content==========\n");
    out.append(mContent);
    if (mContent[mContent.length() - 1] != '\n')
        out.append("\n");

    out.append("###########################\n");

    return out;
}

bool Journal::operator==(Journal &anotherJournal)
{
    hash<string> hFunc;
    size_t       h1 = hFunc(this->toString());
    size_t       h2 = hFunc(anotherJournal.toString());
    if (h1 == h2)
        return true;
    return false;
}