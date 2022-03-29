#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "date.h"
#include "debug_print.h"
#include "Journal.h"
#include "JournalFilter.h"
#include "SfJournalBook.h"
#include "TxtJournalIO.h"
#include "Utils.h"

using namespace std;
using namespace ec;

TEST(Utils, getTagsFormConfig)
{
    shared_ptr<JournalIOBase> journalIO = make_shared<TxtJournalIO>();
    ASSERT_TRUE(journalIO->open("getTagsFormConfig.txt")) << "journal config.txt open fail";
    ASSERT_TRUE(journalIO->setReadMod()) << "setReadMode faild";
    shared_ptr<Journal> journal;
    size_t testNumber=0;
    while (true)
    {
        journal = journalIO->readJournal();
        if (!journal)
            break;
        
        vector<string> tagsFromConfig = getTagsFormConfig(journal->getConfig());
        // expect tag output are in journal content
        vector<string> tags = vector<string>();
        tags.reserve(tagsFromConfig.size());
        istringstream tagsStream = istringstream(journal->getContent());
        string tag;
        while (getline(tagsStream, tag))
        {
            tags.push_back(tag);
        }
        ASSERT_EQ(tagsFromConfig.size(), tags.size()) << string("tags number not eque, on test case ");
        for (size_t i = 0; i < tags.size(); i++)
        {
            ASSERT_TRUE(tagsFromConfig[i] == tags[i]) << string("tags parse error on test case ");
        }
        testNumber++;
        JLOGD("test case %ld pass", testNumber);
    }
    ASSERT_EQ(testNumber, 5) << "not run enough test!!";
}

TEST(Utils, getLevelFormConfig)
{
    shared_ptr<JournalIOBase> journalIO = make_shared<TxtJournalIO>();
    ASSERT_TRUE(journalIO->open("getLevelFormConfig.txt")) << "journal config.txt open fail";
    ASSERT_TRUE(journalIO->setReadMod()) << "setReadMode faild";
    shared_ptr<Journal> journal;
    size_t testNumber=0;
    while (true)
    {
        journal = journalIO->readJournal();
        if (!journal)
            break;
        
        int32_t levelFromConfig = getLevelFormConfig(journal->getConfig());
        // expect tag output are in journal content
        istringstream configStream = istringstream(journal->getContent());
        string levelS;
        int32_t level = -1;
        ASSERT_TRUE(getline(configStream, levelS));
        level = atoi(levelS.c_str());
        ASSERT_EQ(level, levelFromConfig);
        testNumber++;
        JLOGD("test case %ld pass", testNumber);
    }
    ASSERT_EQ(testNumber, 5) << "not run enough test!!";
}

TEST(Filter, stamp)
{
    shared_ptr<JournalBookBase> bookToFilter = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToFilter->open("stampFilter.txt"));
    ASSERT_GE(bookToFilter->size(), 2);
    shared_ptr<JournalBookBase> bookToCmp = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToCmp->open("stampFilterCmp.txt"));
    ASSERT_GE(bookToCmp->size(), 2);

    JournalFilter filter(bookToFilter);
    time_t stFrom = getStampFormConfig(bookToFilter->at(0)->getConfig());
    time_t stTo = getStampFormConfig(bookToFilter->at(1)->getConfig());
    filter.stampFilter(stFrom, false);
    filter.stampFilter(stTo, true);
    filter.sortByStamp();
    vector<size_t> filterOrders = filter.getJournalOrder();
    ASSERT_EQ(filterOrders.size(), bookToCmp->size());
    for (size_t i = 0; i < bookToCmp->size(); i++)
    {
        ASSERT_TRUE(*bookToFilter->at(filterOrders[i])==*bookToCmp->at(i)) << string("faild while i=")+to_string(i)<<bookToFilter->at(filterOrders[i])->toString() << bookToCmp->at(i)->toString();
    }
    
}

TEST(Filter, level)
{
    shared_ptr<JournalBookBase> bookToFilter = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToFilter->open("levelFilter.txt"));
    ASSERT_GE(bookToFilter->size(), 2);
    shared_ptr<JournalBookBase> bookToCmp = make_shared<SfJournalBook>();
    ASSERT_TRUE(bookToCmp->open("levelFilterCmp.txt"));
    ASSERT_GE(bookToCmp->size(), 2);

    JournalFilter filter(bookToFilter);
    int32_t levelFrom = getLevelFormConfig(bookToFilter->at(0)->getConfig());
    int32_t levelTo = getLevelFormConfig(bookToFilter->at(1)->getConfig());
    filter.levelFilter(levelFrom, false);
    filter.levelFilter(levelTo, true);
    filter.sortByLevel();
    vector<size_t> filterOrders = filter.getJournalOrder();
    ASSERT_EQ(filterOrders.size(), bookToCmp->size());
    for (size_t i = 0; i < bookToCmp->size(); i++)
    {
        ASSERT_TRUE(*bookToFilter->at(filterOrders[i])==*bookToCmp->at(i)) << string("faild while i=")+to_string(i)<<bookToFilter->at(filterOrders[i])->toString() << bookToCmp->at(i)->toString();
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    return ret;
}