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
#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Journal.h"
#include "JournalFilter.h"
#include "SfJournalBook.h"
#include "TxtJournalIO.h"
#include "Utils.h"
#include "date.h"
#include "debug_print.h"

using std::string;
using ec::Date;

#define GET_TAG_BOOK     "./testFile/filter_getTagsFormAttributePart.txt"
#define TEG_COUNT        "./testFile/filter_tagsCount.txt"
#define FILTER_TAG_SRC   "./testFile/filter_tagsFilter.txt"
#define FILTER_TAG_DST   "./testFile/filter_tagsFilterCmp.txt"

#define GET_LEVEL_BOOK   "./testFile/filter_getLevelFormAttributePart.txt"
#define FILTER_LEVEL_SRC "./testFile/filter_levelFilter.txt"
#define FILTER_LEVEL_DST "./testFile/filter_levelFilterCmp.txt"

#define FILTER_STAMP_SRC "./testFile/filter_stampFilter.txt"
#define FILTER_STAMP_DST "./testFile/filter_stampFilterCmp.txt"

TEST(Utils, getTagsFormJAttributePart)
{
    shared_ptr<JournalIOBase> journalIO = make_shared<TxtJournalIO>();
    ASSERT_TRUE(journalIO->open(GET_TAG_BOOK))
        << "journal " << GET_TAG_BOOK << " open fail";
    ASSERT_TRUE(journalIO->setReadMod()) << "setReadMode faild";
    shared_ptr<Journal> journal;
    size_t              testNumber = 0;
    while (true)
        {
            journal = journalIO->readJournal();
            if (!journal) break;

            vector<string> tagFromAttributePart =
                getTagsFormJAttributePart(journal->getAttributePart());
            // expect tag output are in journal content
            vector<string> tags = vector<string>();
            tags.reserve(tagFromAttributePart.size());
            istringstream tagsStream = istringstream(journal->getContent());
            string        tag;
            while (getline(tagsStream, tag))
                {
                    tags.push_back(tag);
                }
            ASSERT_EQ(tagFromAttributePart.size(), tags.size())
                << string("tags number not eque, on test case ");
            for (size_t i = 0; i < tags.size(); i++)
                {
                    ASSERT_TRUE(tagFromAttributePart[i] == tags[i])
                        << string("tags parse error on test case ");
                }
            testNumber++;
            JLOGD("test case %ld pass", testNumber);
        }
    ASSERT_EQ(testNumber, 5) << "not run enough test!!";
}

TEST(Utils, getLevelFormJAttributePart)
{
    shared_ptr<JournalIOBase> journalIO = make_shared<TxtJournalIO>();
    ASSERT_TRUE(journalIO->open(GET_LEVEL_BOOK))
        << "journal " << GET_LEVEL_BOOK << " open fail";
    ASSERT_TRUE(journalIO->setReadMod()) << "setReadMode faild";
    shared_ptr<Journal> journal;
    size_t              testNumber = 0;
    while (true)
        {
            journal = journalIO->readJournal();
            if (!journal) break;

            int32_t levelFromAttributePart = getLevelFormJAttributePart(journal->getAttributePart());
            // expect tag output are in journal content
            istringstream attributePartStream = istringstream(journal->getContent());
            string        levelS;
            int32_t       level = -1;
            ASSERT_TRUE(getline(attributePartStream, levelS));
            level = atoi(levelS.c_str());
            ASSERT_EQ(level, levelFromAttributePart);
            testNumber++;
            JLOGD("test case %ld pass", testNumber);
        }
    ASSERT_EQ(testNumber, 5) << "not run enough test!!";
}

TEST(Filter, stamp)
{
    shared_ptr<JournalBookBase> bookToFilter = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToFilter->open(FILTER_STAMP_SRC));
    ASSERT_GE(bookToFilter->size(), 2);
    shared_ptr<JournalBookBase> bookToCmp = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToCmp->open(FILTER_STAMP_DST));
    ASSERT_GE(bookToCmp->size(), 2);

    JournalFilter filter(bookToFilter);
    time_t        stFrom = getStampFormJAttributePart(bookToFilter->at(0)->getAttributePart());
    time_t        stTo   = getStampFormJAttributePart(bookToFilter->at(1)->getAttributePart());
    filter.stampFilter(stFrom, false);
    filter.stampFilter(stTo, true);
    filter.sortByStamp();
    vector<size_t> filterOrders = filter.getJournalOrder();
    ASSERT_EQ(filterOrders.size(), bookToCmp->size());
    for (size_t i = 0; i < bookToCmp->size(); i++)
        {
            ASSERT_TRUE(*bookToFilter->at(filterOrders[i]) == *bookToCmp->at(i))
                << string("faild while i=") + to_string(i)
                << bookToFilter->at(filterOrders[i])->toString()
                << bookToCmp->at(i)->toString();
        }
}

TEST(Filter, level)
{
    shared_ptr<JournalBookBase> bookToFilter = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToFilter->open(FILTER_LEVEL_SRC));
    ASSERT_GE(bookToFilter->size(), 2);
    shared_ptr<JournalBookBase> bookToCmp = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToCmp->open(FILTER_LEVEL_DST));
    ASSERT_GE(bookToCmp->size(), 2);

    JournalFilter filter(bookToFilter);
    int32_t levelFrom = getLevelFormJAttributePart(bookToFilter->at(0)->getAttributePart());
    int32_t levelTo   = getLevelFormJAttributePart(bookToFilter->at(1)->getAttributePart());
    filter.levelFilter(levelFrom, false);
    filter.levelFilter(levelTo, true);
    filter.sortByLevel();
    vector<size_t> filterOrders = filter.getJournalOrder();
    ASSERT_EQ(filterOrders.size(), bookToCmp->size());
    for (size_t i = 0; i < bookToCmp->size(); i++)
        {
            ASSERT_TRUE(*bookToFilter->at(filterOrders[i]) == *bookToCmp->at(i))
                << string("faild while i=") + to_string(i)
                << bookToFilter->at(filterOrders[i])->toString()
                << bookToCmp->at(i)->toString();
        }
}

TEST(Filter, tags)
{
    shared_ptr<JournalBookBase> bookToFilter = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToFilter->open(FILTER_TAG_SRC));
    ASSERT_GE(bookToFilter->size(), 2);
    shared_ptr<JournalBookBase> bookToCmp = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToCmp->open(FILTER_TAG_DST));
    ASSERT_GE(bookToCmp->size(), 2);

    JournalFilter  filter(bookToFilter);
    vector<string> filterTags =
        getTagsFormJAttributePart(bookToFilter->at(0)->getAttributePart());
    filter.withTagsFilter(filterTags);
    vector<size_t> filterOrders = filter.getJournalOrder();
    ASSERT_EQ(filterOrders.size(), bookToCmp->size());
    for (size_t i = 0; i < bookToCmp->size(); i++)
        {
            ASSERT_TRUE(*bookToFilter->at(filterOrders[i]) == *bookToCmp->at(i))
                << string("faild while i=") + to_string(i)
                << bookToFilter->at(filterOrders[i])->toString()
                << bookToCmp->at(i)->toString();
        }
}

TEST(Filter, tagsCount)
{
    shared_ptr<JournalBookBase> book = make_shared<SfJournalBook>();
    ASSERT_TRUE(book->open(TEG_COUNT));
    ASSERT_GE(book->size(), 2);
    JournalFilter       filter(book);
    string              dataStr       = book->at(0)->getContent();
    istringstream       dataStrStream = istringstream(dataStr);
    size_t              i             = 0;
    string              Buffer;
    string              keyStr;
    size_t              number;
    map<string, size_t> mapCmp = map<string, size_t>();
    while (getline(dataStrStream, Buffer))
        {
            if (i % 2)
                {
                    number = atoi(Buffer.c_str());
                    mapCmp.insert(pair<string, size_t>(keyStr, number));
                }
            else
                {
                    keyStr = Buffer;
                }
            i++;
        }
    ASSERT_EQ(i, 6) << "can not read enougth data to verfy count";
    ASSERT_TRUE(mapCmp == filter.tagsCount());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();

    return ret;
}