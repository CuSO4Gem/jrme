#include <stdint.h>
#include <string>

using namespace std;

#include "cmdline.h"
#include "debug_print.h"
#include "JrmeConfig.h"
#include "Utils.h"
#include "WriteMode.h"

int main(int argc, char* argv[]) {
    if (!JrmeConfig::installIfNeed())
    {
        JLOGE("[E] install jrme fail!!\n");
        return -1;
    }
    
    cmdline::parser cmd;
    cmd.add("book", 'b', "select a journal book");
    cmd.add<string>("path", 'p', "input a path of journal book", false, "");
    cmd.add<string>("title", 'T', "The title of journal", false, "");
    cmd.add<string>("at", 'A', "the time of journal", false, "");
    cmd.add<string>("content", 'C', "The content of journal", false, "");

    cmd.parse_check(argc, argv);
    
    string journalBookPath;
    /** Select a journal book;
     */
    if (cmd.exist("book"))
    {
        /** Let user to select a journal book from joruanl book cfg.
         */
        list<string> allBooksPath = JrmeConfig::readJournalBooksCfg();
        if (allBooksPath.size()==0)
        {
            journalBookPath = JrmeConfig::getDefaultJournalBookPath();
            printf("There are no any journal book. Please add journal book by option -a.\n");
            return -1;
        }
        printf("input a number to select a journal book\n");
        size_t i=0;
        for (auto &it:allBooksPath)
        {
            printf("[%ld] %s\n", i,it.c_str());
            i++;
        }
        printf(":");
        string answer;
        getline(cin, answer);
        if (answer.length()==0)
        {
            printf("Invalid input\n");
            return -1;
        }
        int number = atoi(answer.c_str());
        if (number<0 || number>=allBooksPath.size())
        {
            printf("Invalid input\n");
            return -1;
        }
        list<string>::iterator strIt;
        strIt = allBooksPath.begin();
        for (i=0; i<number; i++)
        {
            strIt++;
        }
        journalBookPath = *strIt;
    }
    else if (cmd.exist("path"))
    {
        journalBookPath = cmd.get<string>("path");
        if (journalBookPath.length()==0)
        {
            printf("input a null path\n");
            return -1;
        }
        journalBookPath  = validPath(journalBookPath);
        if (journalBookPath.length()==0)
        {
            printf("option -p or --path get invalid input\n");
            return -1;
        }
    }
    else
        string journalBookPath = JrmeConfig::getDefaultJournalBookPath();
    
    if (cmd.exist("title") || cmd.exist("at") || cmd.exist("content"))
    {
        journlWriteMode(journalBookPath, cmd.get<string>("at"), cmd.get<string>("title"), cmd.get<string>("content"));
        return 0;
    }


    journlWriteMode(journalBookPath, string(""), string(""), string(""));
    return 0;
}