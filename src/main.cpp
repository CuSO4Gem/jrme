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
#include <stdint.h>
#include <string>

using namespace std;

#include "cmdline.h"
#include "debug_print.h"
#include "EnTimeParser.h"
#include "JournalFilter.h"
#include "JrmeConfig.h"
#include "SearchMod.h"
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

    cmd.add<size_t>("number", 'n', "the nomber of journal for show", false);
    cmd.add<string>("from", 'f', "Search time starting point", false);
    cmd.add<string>("to", 't', "Search time ending point", false);
    cmd.add<string>("on", 'o', "Search time on .....", false);
    cmd.add<string>("tags", 'g', "show journal with tags. for example \"first;second;\"", false);
    cmd.add("all_tags", 'G', "show all tags");
    cmd.add<string>("level", 'l', "show level with in level. for example \"-1~10\"", false);

    cmd.add("default_book", 'd', "select default journal book");
    cmd.add<string>("add_book", 'a', "add a journal book path list", false, "");
    cmd.add("edit_book_list", 'B', "edit journal book path list");

    cmd.add("edit", 'E', "edit journal");
    cmd.add("delete", 'D', "delate journal. If input -D true, jrme will delete journal without asking.");
    cmd.add("force_delete", '\0', "force delate journal. If input -D true, jrme will delete journal without asking.");

    cmd.add("version", '\0');

    cmd.parse_check(argc, argv);

    /* print version */
    if (cmd.exist("version"))
    {
#ifndef GIT_TAG_VERSION
        printf("jrme version 0.9.2.\ndebug level %d\n", DBG_PRINT_ENABLED);
#else
        printf("jrme version (get from git) %s \ndebug level %d\n", GIT_TAG_VERSION, DBG_PRINT_ENABLED);
#endif
        return 0;
    }
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
    
    /*write journal*/
    int ret;
    if (cmd.exist("title") || cmd.exist("at") || cmd.exist("content"))
    {
        ret = journlWriteMode(journalBookPath, cmd.get<string>("at"), cmd.get<string>("title"), cmd.get<string>("content"));
        return 0;
    }

    
    if (cmd.exist("all_tags") || cmd.exist("tags")||
        cmd.exist("level")    || cmd.exist("on")  ||
        cmd.exist("from")     || cmd.exist("to")  ||
        cmd.exist("number"))
        ret = journalSearchMod(cmd, journalBookPath);
    else
        ret = journlWriteMode(journalBookPath, string(""), string(""), string(""));
    

    return ret;
}