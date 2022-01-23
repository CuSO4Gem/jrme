#include <gtest/gtest.h>
#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include <syscall.h>

#include "Journal.h"
#include "TxtJournalIO.h"
#include "JournalIOBase.h"

TEST(TxtJournalIO, read)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed";
    TxtJournalIO journalIO;
    ASSERT_TRUE(journalIO.openJournal("read1.txt")) << "TxtJournalIO open failed";
    journalIO.setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> gotJournal = journalIO.readJournal();
        ASSERT_TRUE(gotJournal!=nullptr) << "journalIO read faild";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        ASSERT_TRUE(*gotJournal == jStandard) << "fail in loop " << to_string(i);
    }
}
/*
TEST(TxtJournalIO, write)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed";
    TxtJournalIO journalIO;
    ASSERT_TRUE(journalIO.openJournal("test.txt")) << "TxtJournalIO open failed";
    journalIO.setWriteMode();
    list<shared_ptr<Journal>> journalList;
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO.readJournal();
        ASSERT_TRUE(gotJournal!=nullptr) << "journalIO read faild";
        journalList.push_back(journal);
    }
    

}*/

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    
    return ret;
}