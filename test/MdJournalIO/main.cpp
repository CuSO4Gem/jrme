#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include "Journal.h"
#include "PluginJournalIO.h"
#include "JournalIOBase.h"

TEST(MdJournalIO, read)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed";
    shared_ptr<PluginJournalIO> journalIO = make_shared<PluginJournalIO>();
    ASSERT_TRUE(journalIO->loadPlugin("md_journal.so")) << "load journalIO plugin fail";
    ASSERT_TRUE(journalIO->open("read1.md")) << "MdJournalIO open failed";
    journalIO->setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO->readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read fail";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        ASSERT_TRUE(*journal == jStandard) << "fail in loop " << to_string(i) << "\n" << journal->toString() << jStandard.toString();
    }
}

TEST(MdJournalIO, write)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    
    shared_ptr<PluginJournalIO> journalIO = make_shared<PluginJournalIO>();
    ASSERT_TRUE(journalIO->loadPlugin("md_journal.so")) << "load journalIO plugin fail";
    string fileToWrite = string("test.txt");
    remove(fileToWrite.c_str());
    ASSERT_TRUE(journalIO->open(fileToWrite)) << "MdJournalIO open failed";
    bool ret = journalIO->setWriteMode();
    for (size_t i = 0; i < 3; i++)
    {
        string lineBuffer;
        shared_ptr<Journal> jStandard = shared_ptr<Journal>(new Journal);
        getline(journalStore, lineBuffer, '#');
        jStandard->setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard->setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard->setContent(lineBuffer);
        
        bool writeRet = journalIO->writeJournal(jStandard);
        ASSERT_TRUE(writeRet) << "journalIO wirte file:" << fileToWrite << " failed";
    }
    
    /* verfy writen file */
    journalStore.seekg(0, ios::beg);
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    ASSERT_TRUE(journalIO->open(fileToWrite)) << "MdJournalIO open failed";
    journalIO->setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO->readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read fail when verfy write";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        if (!(*journal == jStandard))
        {
            printf("%s",journal->toString().c_str());
            printf("%s",jStandard.toString().c_str());
        }
        ASSERT_TRUE(*journal == jStandard) << " verfy write fail in loop " << to_string(i);
    }
}

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    
    return ret;
}