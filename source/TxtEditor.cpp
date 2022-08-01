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
#include "TxtEditor.h"
#include "JrmeConfig.h"
#include "iniparser.hpp"
#include "stdlib.h"
#include <fstream>
#include <regex>
#include <sstream>

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
    int                 ret;

    /** 让用户将数据输入到一个临时文件。该文件位于jrme配置文件根目录
     * Let user input data to a temporary file. The file is in the root dir of jrme config file.
     */
    string   tmpFilePath = JrmeConfig::getConfigRootDir() + ".journal.tmp";
    ofstream oTmpFile;
    oTmpFile.open(tmpFilePath);
    oTmpFile << mInitStr << endl;
    oTmpFile.close();

    /** 调用配置文件中指定的文本编辑器
     * Call the text editor specified in the config file
     */
    string cmdEditor = string();
    cmdEditor = JrmeConfig::getEditorName();
    ret       = system((cmdEditor + " " + tmpFilePath).c_str());

    /** 用户写入完成后，逐行读取临时文件，并将其内容放入Journal对象中
     * When user finished writing, read the temporary file line by line and put the content into Journal object
     */
    ifstream iTmpFile;
    iTmpFile.open(tmpFilePath);
    string lineBuffer;
    string readBuffer;
    smatch regexResult;
    bool   finded;
    if (!getline(iTmpFile, lineBuffer))
        goto fail;
    if (!regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}journal[ ]{0,}={2,}")))
        goto fail;

    finded = false;
    while (getline(iTmpFile, lineBuffer))
    {
        size_t prLen = 0;
        if (regex_search(lineBuffer, regexResult, regex("^={2,}[ ]{0,}attributePart[ ]{0,}={2,}")))
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
        size_t prLen = 0;
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
        journal->setAttributePart(readBuffer);

    readBuffer.clear();
    lineBuffer.clear();
    while (getline(iTmpFile, lineBuffer))
    {
        readBuffer.append(lineBuffer + string("\n"));
    }
    journal->setContent(readBuffer);

    iTmpFile.close();
    remove(tmpFilePath.c_str());
    return journal;

fail:
    iTmpFile.close();
    remove(tmpFilePath.c_str());
    return nullptr;
}