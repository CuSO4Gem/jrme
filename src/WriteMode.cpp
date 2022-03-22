#include <fstream>
#include <memory>
#include <iostream>
#include <sys/stat.h>

#include "ConfigNodeMaster.h"
#include "date.h"
#include "DateConfigNode.h"
#include "debug_print.h"
#include "EnTimeParser.h"
#include "iniparser.hpp"
#include "JrmeConfig.h"
#include "TagConfigNode.h"
#include "TxtEditor.h"
#include "LevelConfigNode.h"
#include "SfJournalBook.h"
#include "WriteMode.h"

using namespace std;
using namespace ec;

void journlWriteMode(string bookPath, string timeDescription, string title, string content)
{
    shared_ptr<JournalBookBase> journalBook;

    struct stat sBuf;
    stat(bookPath.c_str(), &sBuf);
    if (S_ISDIR(sBuf.st_mode))
    {
        //todo : multifile journal book
        ;
    }
    else
        journalBook = make_shared<SfJournalBook>();

    if(!journalBook->open(bookPath))
    {
        JLOGE("[E] conn't not open journal book %s\n", bookPath.c_str());
        return;
    }

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
    INI::File configFile = INI::File(JrmeConfig::getConfigFilePath());
    string plugNames = configFile.GetSection("plugin")->GetValue("config node").AsString();
    istringstream plugNameStream = istringstream(plugNames);
    string pluginName;
    bool load_fail = false;
    while (getline(plugNameStream, pluginName, ','))
    {
        if (!configMaster.addPluginNode(pluginName))
        {
            printf("warning: plugin %s not find\n", pluginName.c_str());
            load_fail = true;
        }
    }
    if (load_fail)
    {
        string tmpStr;
        getline(cin, tmpStr);
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

    configMaster.postprocess(journal);
    
    journalBook->push_back(journal);
    journalBook->order();
    journalBook->save();
}
