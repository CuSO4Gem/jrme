#include <fstream>
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <unistd.h>

#include "ConfigNodeMaster.h"
#include "date.h"
#include "DateConfigNode.h"
#include "debug_print.h"
#include "EnTimeParser.h"
#include "iniparser.hpp"
#include "JrmeConfig.h"
#include "LevelConfigNode.h"
#include "pthread.h"
#include "SfJournalBook.h"
#include "TagConfigNode.h"
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

void *openJournalBook(void* args)
{
    struct stat sBuf;
    stat(sBookPath.c_str(), &sBuf);
    if (S_ISDIR(sBuf.st_mode))
    {
        //todo : multifile journal book
        ;
    }
    else
        sJournalBook = make_shared<SfJournalBook>();

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

void journlWriteMode(string bookPath, string timeDescription, string title, string content)
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
            JLOGT("[T] spend too mach time in opening journal book!");
        }
        else
        {
            openFinishMu.unlock();
            // before out of time, we get result but journal book open fail.
            if (!sJournalBook)
            {
                printf("connot not open journal book %s", bookPath.c_str());
                return;
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
    ConfigNodeMaster configMaster = ConfigNodeMaster();
    Date date;
    if (timeDescription.length()==0)
        date = Time().toDate();
    else
    {
        EnTimeParser parser;
        timeParserRet timeRet = parser.parse(timeDescription);
        date.setYear(timeRet.year);
        date.setMonth(timeRet.month);
        date.setDay(timeRet.day);
        date.setHour(timeRet.hour);
        date.setMinute(timeRet.minute);
        date.setSecond(timeRet.second);
    }
    configMaster.setDate(date.stamp());
    // load config node plugin
    vector<string> pluginNameVector = JrmeConfig::getConfigNodePluginNames();
    string pluginName;
    for (auto &it:pluginNameVector)
    {
        if (!configMaster.addPluginNode(it))
        {
            printf("warning: plugin %s not find\n", pluginName.c_str());
        }
    }
    

    string config = configMaster.genConfig();
    
    shared_ptr<Journal> journal = make_shared<Journal>();
    journal->setTitle(title);
    journal->setConfig(config);
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
        strBuffer.append("==========config==========\n");
        strBuffer.append(journal->getConfig());
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
            return ;
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
        return;
    }
#endif
    sJournalBook->push_back(journal);
    sJournalBook->order();
    sJournalBook->save();
    JLOGD("[D] write a journal to %s", bookPath.c_str());
}
