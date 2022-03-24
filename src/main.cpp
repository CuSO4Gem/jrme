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
    cmd.add<string>("title", 'T', "The title of journal", false, "");
    cmd.add<string>("at", 'A', "the time of journal", false, "");
    cmd.add<string>("content", 'C', "The content of journal", false, "");
    cmd.add("book", 'b', "select a journal book");
    cmd.add<string>("path", 'p', "input a path of journal book", false, "");

    cmd.add("default_book", 'd', "select default journal book");
    cmd.add<string>("add_book", 'a', "add a journal book path list", false, "");
    cmd.add("edit_book_list", 'B', "edit journal book path list");
    cmd.parse_check(argc, argv);

    /*Select a journal book as default*/
    if (cmd.exist("default_book"))
    {
        list<string> allBooksPath = JrmeConfig::readJournalBooksCfg();
        printf("input a number to select the default journal book\n");
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
            return 0;
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
        JrmeConfig::setDeafultJournalBookPath(*strIt);
        return 0;
    }
    
    /*Add book path to journal book path list*/
    if (cmd.exist("add_book"))
    {
        string path = cmd.get<string>("add_book");
        path = validPath(path);
        if (path.size()==0)
        {
            printf("Invalid input\n");
            return -1;
        }
        list<string> pathList = JrmeConfig::readJournalBooksCfg();
        pathList.push_back(path);
        JrmeConfig::writeJournalBooksCfg(pathList);
        return 0;
    }

    /*Edit journal book path list config file*/
    if (cmd.exist("edit_book_list"))
    {
        string editorName = JrmeConfig::getEditorName();
        string bookListCfg = JrmeConfig::getJournalBooksCfgPath();
        system((editorName + " " + bookListCfg).c_str());
        return 0;
    }

    /** Select a journal book */
    string journalBookPath;
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
        if (answer.length()==0 || !isNumChar(answer[0]))
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
        journalBookPath = JrmeConfig::getDefaultJournalBookPath();
        
    if (cmd.exist("title") || cmd.exist("at") || cmd.exist("content"))
    {
        journlWriteMode(journalBookPath, cmd.get<string>("at"), cmd.get<string>("title"), cmd.get<string>("content"));
        return 0;
    }


    journlWriteMode(journalBookPath, string(""), string(""), string(""));
    return 0;
}