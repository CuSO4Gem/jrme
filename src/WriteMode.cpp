#include <fstream>
#include <memory>
#include <sys/stat.h>

#include "ConfigNodeMaster.h"
#include "DateConfigNode.h"
#include "date.h"
#include "EnTimeParser.h"
#include "iniparser.hpp"
#include "JrmeConfig.h"
#include "TagConfigNode.h"
#include "KiloEditor.h"
#include "LevelConfigNode.h"
#include "SfJournalBook.h"
#include "WriteMode.h"

using namespace ec;

void writeJournal(string bookPath, string timeDescription, string title)
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
        printf("error: conn't not open journal book %s\n", bookPath.c_str());
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
    INI::File configFile = INI::File(getConfigFilePath());
    string plugNames = configFile.GetSection("plugin")->GetValue("config node").AsString();
    istringstream plugNameStream = istringstream(plugNames);
    string pluginName;
    while (getline(plugNameStream, pluginName, ','))
    {
        configMaster.addPluginNode(pluginName);
    }

    string config = configMaster.genConfig();
    
    shared_ptr<Journal> journal = make_shared<Journal>();
    journal->setTitle(title);
    journal->setConfig(config);
    configMaster.preprocess(journal);

    string strBuffer;
    strBuffer.append("==========journal==========\n");
    strBuffer.append(journal->getTitle());
    strBuffer.append(" \n");
    strBuffer.append("==========config==========\n");
    strBuffer.append(journal->getConfig());
    strBuffer.append("==========content==========");

    TxtEditor editor;
    editor.setInitStr(strBuffer);
    string gotStr;
    if (!editor.getTextFromEditor(gotStr))
        return ;
    
    journal = strToJournal(gotStr);
    configMaster.postprocess(journal);
    
    journalBook->push_back(journal);
    journalBook->order();
    journalBook->save();
    system("clear");
}
