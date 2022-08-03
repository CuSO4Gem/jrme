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
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <syscall.h>

using std::string;

#include "SfJournalBook.h"
#include "Utils.h"

#include "debug_print.h"

#define JOURNAL_INSERT_JOURNAL_SORTE "./testFile/SfJournalBook_insertSort.txt"
#define JOURNAL_SOURCE_BOOK          "./testFile/SfJournalBook_book.txt"
#define JOURNAL_TARGET_BOOK          "./testFile/SfJournalBook_tmp.txt"

TEST (SFJournalBook, total)
{
    ifstream journalStore;
    char     cmdStr[512];
    int      ret;

    sprintf (cmdStr, "cp %s %s", JOURNAL_SOURCE_BOOK, JOURNAL_TARGET_BOOK);
    ret = system (cmdStr);
    journalStore.open (JOURNAL_INSERT_JOURNAL_SORTE);
    ASSERT_TRUE (journalStore.is_open ())
        << JOURNAL_INSERT_JOURNAL_SORTE << " open failed";
    SfJournalBook journalBook;
    ASSERT_TRUE (journalBook.open (JOURNAL_TARGET_BOOK))
        << "journal book open filed";
    ASSERT_EQ (journalBook.size (), 3)
        << "journal on journal book" << JOURNAL_TARGET_BOOK << "not eq 3";

    string              lineBuffer;
    shared_ptr<Journal> jInsert = shared_ptr<Journal> (new Journal);
    getline (journalStore, lineBuffer, '#');
    jInsert->setTitle (lineBuffer);
    getline (journalStore, lineBuffer, '#');
    jInsert->setAttributePart (lineBuffer);
    getline (journalStore, lineBuffer, '#');
    jInsert->setContent (lineBuffer);

    journalBook.push_back (jInsert);
    ASSERT_EQ (journalBook.size (), 4)
        << "journal on SfJournalBook not eq 4 after insert";

    journalBook.order ();
    ASSERT_TRUE (journalBook.save ()) << "journal book save fail";
    journalBook.close ();

    // verfy
    SfJournalBook journalBookV;
    ASSERT_TRUE (journalBookV.open (JOURNAL_TARGET_BOOK))
        << "journal book open filed while verfy";
    ASSERT_EQ (journalBookV.size (), 4)
        << "journal on journal book test.txt not eq 4 while verfy";
    bool   inorder    = true;
    size_t loopCount  = journalBookV.size ();
    time_t stampBefor = getStampFormJAttributePart (journalBookV.at (0)->getAttributePart ());
    time_t stampNow;
    for (size_t i = 1; i < loopCount; i++)
        {
            stampNow = getStampFormJAttributePart (journalBookV.at (i)->getAttributePart ());
            if (stampBefor < stampNow)
                {
                    inorder = false;
                    break;
                }
            stampBefor = stampNow;
        }
    ASSERT_TRUE (inorder) << "journal no in order";
}

int
main (int argc, char *argv[])
{

    testing::InitGoogleTest (&argc, argv);
    int ret = RUN_ALL_TESTS ();
    return ret;
}