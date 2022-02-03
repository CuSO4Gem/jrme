#ifndef JOURNAL_IO_BASE_H
#define JOURNAL_IO_BASE_H

#include <list>
#include <string>

using namespace std;

class JournalIOBase
{
public:
    virtual ~JournalIOBase() = default;

    /**
     * @brief support api version
     * 
     * @return uint32_t api version
     */
    virtual uint32_t apiSupport() = 0;

    /**
     * @brief support format
     * 
     * @return list<string> formate list
     */
    virtual list<string> formateSupport() = 0;

    /**
     * @brief open file and ready to parse
     * 
     * @param path the journal path
     * @return true open success
     * @return false open failed
     */
    virtual bool openJournal(string path) = 0;

    /**
     * @brief set read mod, it can read journal
     * 
     * @return true 
     * @return false 
     */
    virtual bool setReadMod() = 0;

    /**
     * @brief set write mod, it can write journal
     * 
     * @return true 
     * @return false 
     */
    virtual bool setWriteMode() = 0;
    /**
     * @brief read and parser from file
     * 
     * @param title 
     * @param config 
     * @param content 
     * @return true read success
     * @return false read fail
     */
    virtual bool readJournal(string &title, string &config, string &content) = 0;

    /**
     * @brief write journal to file
     * 
     * @param title 
     * @param config 
     * @param content 
     * @return true 
     * @return false 
     */
    virtual bool writeJournal(const string &title, const string &config, const string &content) = 0;
};
#endif