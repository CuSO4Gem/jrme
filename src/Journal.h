#ifndef JOURNAL_H
#define JOURNAL_H

#include <string>

using namespace std;

class Journal
{
private:
    string mTitle;
    string mConfig;
    string mContent;

public:
    Journal() = default;
    Journal(const string &title, const string &config, const string &content);
    ~Journal() = default;

    void setTitle(const string &title);
    void setConfig(const string &config);
    void setContent(const string &content);
    string getTitle();
    string getConfig();
    string getContent();

    void clear();
    string toString();
};

#endif