#include <gtest/gtest.h>
#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include "Journal.h"
#include "TxtJournalIO.h"
#include "JournalIOBase.h"

TEST(TxtJournalIO, read)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed";
    TxtJournalIO journalIO;
    ASSERT_TRUE(journalIO.open("read1.txt")) << "TxtJournalIO open failed";
    journalIO.setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO.readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read faild";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        ASSERT_TRUE(*journal == jStandard) << "fail in loop " << to_string(i);
    }
}

TEST(TxtJournalIO, write)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    
    TxtJournalIO journalIO;
    string fileToWrite = string("test.txt");
    ASSERT_TRUE(journalIO.open(fileToWrite)) << "TxtJournalIO open failed";
    bool ret = journalIO.setWriteMode();
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
        
        bool writeRet = journalIO.writeJournal(jStandard);
        ASSERT_TRUE(writeRet) << "journalIO wirte file:" << fileToWrite << " failed";
    }
    
    /* verfy writen file */
    journalStore.seekg(0, ios::beg);
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    journalIO.setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO.readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read faild when verfy write";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        ASSERT_TRUE(*journal == jStandard) << " verfy write fail in loop " << to_string(i);
    }
}

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    
    return ret;
}