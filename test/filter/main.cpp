#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "debug_print.h"
#include "Journal.h"
#include "TxtJournalIO.h"
#include "Utils.h"

using namespace std;

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

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    return ret;
}