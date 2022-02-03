#include <gtest/gtest.h>
#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include "date.h"
#include "DateConfigNode.h"
#include "Journal.h"
#include "utils.h"

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

    node.beforeInput(journal);
    node.afterInput(journal);
    
    ASSERT_TRUE(date.stamp() == getStampFormConfig(journal->getConfig()));
}

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    printf("hellow!!\n");
    return ret;
}