#include <fstream>
#include "iniparser.hpp"
#include <regex>
#include "stdlib.h"
#include <sstream>
#include "TxtEditor.h"
#include "JrmeConfig.h"

TxtEditor::TxtEditor(string initStr)
{
    mInitStr = initStr;
}

void TxtEditor::setInitStr(string initStr)
{
    mInitStr = initStr;
}

void TxtEditor::clearInitStr()
{
    mInitStr = "";
}

shared_ptr<Journal> TxtEditor::getJournalFromEditor()
{
    shared_ptr<Journal> journal = make_shared<Journal>();

    string tmpFilePath = JrmeConfig::getConfigRootDir()+".journal.tmp";
    ofstream oTmpFile;
    oTmpFile.open(tmpFilePath);
    oTmpFile << mInitStr << endl;
    oTmpFile.close();

    string cmdEditor = JrmeConfig::getEditorName();
    system((cmdEditor + " " + tmpFilePath).c_str());

    ifstream iTmpFile;
    iTmpFile.open(tmpFilePath);

    string lineBuffer;
    string readBuffer;
    smatch regexResult;
    bool finded;
    if (!getline(iTmpFile, lineBuffer))
        goto fail;
    if (!regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        goto fail;

    finded = false;
    while (getline(iTmpFile, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}config[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
        goto fail;
    else
        journal->setTitle(readBuffer);

    readBuffer.clear();
    finded = false;
    while (getline(iTmpFile, lineBuffer))
    {
        size_t prLen=0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}content[ ]{0,}={2,}")))
        {
            finded = true;
            break;
        }
        else
            readBuffer.append(lineBuffer + string("\n"));
    }
    if (!finded)
        goto fail;
    else
        journal->setConfig(readBuffer);
    
    readBuffer.clear();
    iTmpFile >> readBuffer;
    journal->setContent(readBuffer);

    iTmpFile.close();
    remove(tmpFilePath.c_str());
    return journal;

fail:
    iTmpFile.close();
    remove(tmpFilePath.c_str());
    return nullptr;
}