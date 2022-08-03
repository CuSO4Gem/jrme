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
#ifndef JOURNAL_H
#define JOURNAL_H

#include <string>

using std::string;

/**
 * @brief
 * 一篇日记
 * a journal
 */
class Journal
{
public:
    explicit Journal() = default;
    explicit Journal(const string &title, const string &attributePart, const string &content);
    ~Journal() = default;

    void    setTitle(const string &title);
    void    setAttributePart(const string &attributePart);
    void    setContent(const string &content);
    string  getTitle();
    string &getAttributePart();
    string  getContent();

    void   clear();
    string toString();

    bool operator==(Journal &anotherJournal);

private:
    string mTitle;
    string mAttributePart;
    string mContent;
};

#endif