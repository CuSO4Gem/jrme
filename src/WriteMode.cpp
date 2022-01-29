#include <fstream>
#include <memory>
#include <sys/stat.h>

#include "ConfigNodeMaster.h"
#include "date.h"
#include "KiloEditor.h"
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

    // todo : time parse from timeDescription
    Date date = Time().toDate();

    ConfigNodeMaster configMaster = ConfigNodeMaster();
    configMaster.setDate(date.stamp());
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
    strBuffer.append("==========content==========\n  ");

    TxtEditor editor;
    editor.setInitStr(strBuffer);
    string gotStr;
    if (!editor.getTextFromEditor(gotStr))
        return ;
    
    journal = strToJournal(gotStr);
    journalBook->push_back(journal);
    journalBook->order();
    journalBook->save();
    system("clear");
}
