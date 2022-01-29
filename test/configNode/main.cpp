#include <gtest/gtest.h>
#include <list>
#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include "ConfigNodeMaster.h"
#include "date.h"
#include "DateConfigNode.h"
#include "Journal.h"
#include "Utils.h"

using namespace ec;

TEST(DateConfigNode, basic)
{
    DateConfigNode node;
    Date date;
    bool retBool;

    ASSERT_TRUE(node.getKey() == string("date")) << "config node key not right";
    string title = string("a temp journal to test");
    string config = string("");
    string content = string("no more content");
    shared_ptr<Journal> journal = make_shared<Journal>();
    journal->setTitle(title);
    journal->setConfig(config);
    journal->setContent(content);

    date = Date(1970, 1, 1, 9, 0, 0);
    node.setDate(date.stamp());
    journal->getConfig().append(string("tag=NULL\n"));
    journal->getConfig().append(node.getKey()+"="+node.getDefaultValue()+"\n");
    journal->getConfig().append(string("level=0\n"));

    node.preprocess(journal);
    node.postprocess(journal);
    
    ASSERT_TRUE(date.stamp() == getStampFormConfig(journal->getConfig()));
}

TEST(ConfigNodeMaster, InnerKeyGen)
{
    list<string> keyList;
    keyList.push_back(string("date="));
    keyList.push_back(string("tag="));
    keyList.push_back(string("level="));

    ConfigNodeMaster nodeMaster;

    string config = nodeMaster.genConfig();

    uint32_t findedCount=0;
    uint32_t failCoung=0;
    for (auto &it:keyList)
    {
        if (config.find(it) != string::npos)
            findedCount++;
        else
            failCoung++;
    }
    ASSERT_TRUE(findedCount==keyList.size()) << "some inner key not find while Config generate";    
}

TEST(ConfigNodeMaster, InnerPreprocess)
{
    shared_ptr<Journal> journal = make_shared<Journal>();

    ConfigNodeMaster nodeMaster;
    string config = nodeMaster.genConfig();
    journal->setConfig(config);
    journal->setTitle(string("config node master"));
    nodeMaster.preprocess(journal);

    config = journal->getConfig();
    string key = string("date");
    string value;
    getValueFromConfig(config, key, value);
    ASSERT_NE(value.length(), 0);
}

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    printf("hellow!!\n");
    return ret;
}