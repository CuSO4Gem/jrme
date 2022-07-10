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
#include <fstream>
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <unistd.h>

#include "JAttributeMaster.h"
#include "date.h"
#include "DataJAttribute.h"
#include "debug_print.h"
#include "EnTimeParser.h"
#include "iniparser.hpp"
#include "JrmeConfig.h"
#include "LevelJAttribute.h"
#include "pthread.h"
#include "SfJournalBook.h"
#include "TagJAttribute.h"
#include "TxtEditor.h"
#include "Utils.h"
#include "WriteMode.h"

using namespace std;
using namespace ec;

//create a pthread to open journal book
#define PTHREAD_OPEN
#ifdef PTHREAD_OPEN
timed_mutex openFinishMu;
#endif

static shared_ptr<JournalBookBase> sJournalBook = nullptr;
static string sBookPath;

/**
 * @brief 
 * 加载日记本，可以多线程加载
 * Load journal book. Can run as pthread
 */
void *openJournalBook(void* args)
{
    sJournalBook = bookFactory(sBookPath);
    if (!sJournalBook)
        return NULL;
        
    if(!sJournalBook->open(sBookPath))
    {
        JLOGE("[E] connot not open journal book %s", sBookPath.c_str());
        sJournalBook = nullptr;
    }

    #ifdef PTHREAD_OPEN
    // just for nofiy that the process of open jounal book have finish.
    openFinishMu.unlock();
    #endif
    return NULL;
}

int journlWriteMode(string bookPath, string timeDescription, string title, string content)
{
    sBookPath = bookPath;
#ifdef PTHREAD_OPEN
    pthread_t openTid=0;
    openFinishMu.lock();
    pthread_create(&openTid, NULL, openJournalBook, NULL);
    /**
     * The process of open journal book my fail. The user would be sad, that
     * if user has finish input but find the journal book opened fail.  
     * If the journal book can be open in 50 ms, that would be fine.
     * If time, title and content are all input from commend line, don't wait.
     */
    if (!(title.length()>0 && timeDescription.length()>0 && content.length()>0))
    {
        if (!openFinishMu.try_lock_for(chrono::milliseconds(50)))
        {
            JLOGI("[I] spend too mach time in opening journal book!");
        }
        else
        {
            openFinishMu.unlock();
            // before out of time, we get result but journal book open fail.
            if (!sJournalBook)
            {
                printf("connot not open journal book %s", bookPath.c_str());
                return -1;
            }
        }
    }
#else
    openJournalBook(NULL);
    /* sJournalBook is nullptr while journal book open fail. return immediately*/
    if (!sJournalBook)
    {
        printf("connot not open journal book %s", bookPath.c_str());
        return;
    }
#endif

    // todo : more type of time parse from timeDescription
    JAttributeMaster configMaster = JAttributeMaster();
    Date date;
    if (timeDescription.length()==0)
        date = Time().toDate();
    else
    {
        EnTimeParser parser;
        timeParserRet timeRet = parser.parse(timeDescription);
        if (timeRet.estimation<TIME_PARSE_LITTLE_SUCCESS || timeRet.flag==0)
        {
            JLOGE("EnTimeParser faild");
            date = Time().toDate();
        }
        else
        {
            date.setYear(timeRet.year);
            date.setMonth(timeRet.month);
            date.setDay(timeRet.day);
            date.setHour(timeRet.hour);
            date.setMinute(timeRet.minute);
            date.setSecond(timeRet.second);
        }
    }
    configMaster.setDate(date.stamp());
    // load JAttribute plugin
    list<string> pluginNameVector = JrmeConfig::getConfigNodePluginNames();
    string pluginName;
    for (auto &it:pluginNameVector)
    {
        if (!configMaster.addPluginNode(it))
        {
            printf("warning: plugin %s not find\n", pluginName.c_str());
        }
    }
    

    string attributePart = configMaster.genJAttributePart();
    
    shared_ptr<Journal> journal = make_shared<Journal>();
    journal->setTitle(title);
    journal->setAttributePart(attributePart);
    journal->setContent(content);
    configMaster.preprocess(journal);

    /** no all of title, time and content are ready, program will call editor to get a jounal.
     * Otherwise, journal will be save directly.
     * */
    if (!(title.length()>0 && timeDescription.length()>0 && content.length()>0))
    {
        string strBuffer;
        strBuffer.append("==========journal==========\n");
        strBuffer.append(journal->getTitle());
        strBuffer.append("\n");
        strBuffer.append("=======attributePart=======\n");
        strBuffer.append(journal->getAttributePart());
        strBuffer.append("==========content==========\n");
        strBuffer.append(journal->getContent());
        TxtEditor editor;
        editor.setInitStr(strBuffer);
        string gotStr;
        journal = nullptr;
        journal = editor.getJournalFromEditor();
        if (!journal)
        {
            JLOGE("[E] The input journal format error!!\n");
            return -1;
        }
    }
    void *tRet;
    configMaster.postprocess(journal);
    
    pthread_join(openTid, &tRet);
#ifdef PTHREAD_OPEN
    /** 
     * Program has created a pthread to open jounal book. Program my find that,
     * user has finish input a journal, but journal book open fail. Quary user, if
     * he want to save journal to another path
    */
    if (sJournalBook==nullptr)
    {
        printf("journal book open fail, would you like to save the jounal which input just befor to another place? (Y/N):");
        bool wantSave = true;
        string gotAnswer;
        getline(cin, gotAnswer);
        if (!(gotAnswer[0] == 'Y' || gotAnswer[0] == 'y'))
        {
            wantSave = false;
        }
        while (wantSave)
        {
            printf("input path:");
            gotAnswer = getFilePath();
            ofstream outFile;
            outFile.open(gotAnswer, ios::out);
            if (!outFile.is_open())
            {
                printf("file open faild:%s\n",strerror(errno));
                printf("would you want to save to another path? (Y/N):");
                getline(cin, gotAnswer);
                if (!(gotAnswer[0] == 'Y' || gotAnswer[0] == 'y'))
                {
                    wantSave = false;
                }
            }
            else
            {
                outFile << journal->toString();
                outFile.close();
                break;
            }
        }
        return 0;
    }
#endif
    sJournalBook->push_back(journal);
    sJournalBook->order();
    sJournalBook->save();
    JLOGD("[D] write a journal to %s", bookPath.c_str());
    
    return 0;
}
