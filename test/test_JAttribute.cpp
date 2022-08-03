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
#include <list>
#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using std::string;

#include "JAttributeMaster.h"
#include "date.h"
#include "DataJAttribute.h"
#include "Journal.h"
#include "Utils.h"

using ec::Date;

TEST(DataJAttribute, basic)
{
    DataJAttribute node;
    Date date;
    bool retBool;

    ASSERT_TRUE(node.getKey() == string("date")) << "JAttribute key not right";
    string title = string("a temp journal to test");
    string attributePart = string("");
    string content = string("no more content");
    shared_ptr<Journal> journal = make_shared<Journal>();
    journal->setTitle(title);
    journal->setAttributePart(attributePart);
    journal->setContent(content);

    date = Date(1970, 1, 1, 9, 0, 0);
    node.setDate(date.stamp());
    journal->getAttributePart().append(string("tags=NULL\n"));
    journal->getAttributePart().append(node.getKey()+"="+node.getDefaultValue()+"\n");
    journal->getAttributePart().append(string("level=0\n"));

    node.preprocess(journal);
    node.postprocess(journal);
    
    ASSERT_TRUE(date.stamp() == getStampFormJAttributePart(journal->getAttributePart()));
}

TEST(JAttributeMaster, InnerKeyGen)
{
    list<string> keyList;
    keyList.push_back(string("date="));
    keyList.push_back(string("tags="));
    keyList.push_back(string("level="));

    JAttributeMaster nodeMaster;

    string attributePart = nodeMaster.genJAttributePart();

    uint32_t findedCount=0;
    uint32_t failCoung=0;
    for (auto &it:keyList)
    {
        if (attributePart.find(it) != string::npos)
            findedCount++;
        else
            failCoung++;
    }
    ASSERT_TRUE(findedCount==keyList.size()) << "some inner key not find while attributePart generate";    
}

TEST(JAttributeMaster, InnerPreprocess)
{
    shared_ptr<Journal> journal = make_shared<Journal>();

    JAttributeMaster nodeMaster;
    string attributePart = nodeMaster.genJAttributePart();
    journal->setAttributePart(attributePart);
    journal->setTitle(string("attributePart master"));
    nodeMaster.preprocess(journal);

    attributePart = journal->getAttributePart();
    string key = string("date");
    string value;
    getValueFromJAttributePart(attributePart, key, value);
    ASSERT_NE(value.length(), 0);
}

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    return ret;
}