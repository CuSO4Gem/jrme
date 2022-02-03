#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <syscall.h>

using namespace std;


#include "SfJournalBook.h"

TEST(SFJournalBook, total)
{
    ifstream journalStore;
    journalStore.open("loop1Insert.txt");
    ASSERT_TRUE(journalStore.is_open()) << "loop1Insert.txt open failed";
    system("cp loop1.txt test.txt");
    SfJournalBook journalBook;
    ASSERT_TRUE(journalBook.open("test.txt")) << "journal book open filed";

    string lineBuffer;
    shared_ptr<Journal> jInsert = shared_ptr<Journal>(new Journal);
    getline(journalStore, lineBuffer, '#');
    jInsert->setTitle(lineBuffer);
    getline(journalStore, lineBuffer, '#');
    jInsert->setConfig(lineBuffer);
    getline(journalStore, lineBuffer, '#');
    jInsert->setContent(lineBuffer);

    journalBook.push_back(jInsert);
    journalBook.order();
    journalBook.save();
}


int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 
    return ret;
}