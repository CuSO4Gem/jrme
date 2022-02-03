#include "DateConfigNode.h"
#include "utils.h"

DateConfigNode::DateConfigNode()
{
    mDate = Date(1970, 1, 1, 9, 0, 0);
}

uint32_t DateConfigNode::apiVersion()
{
    return 1;
}

string DateConfigNode::getKey()
{
    return string("date");
}

string DateConfigNode::getDefaultValue()
{
    return string("");
}

void DateConfigNode::beforeInput(shared_ptr<Journal> journal)
{
    //todo : from timeparser
    setValueToConfig(journal->getConfig(), "date", mDate.toString());
}

void DateConfigNode::afterInput(shared_ptr<Journal> journal)
{
    return;
}

int32_t DateConfigNode::innerConfigType()
{
    return DATE_CONFIG;
}

void DateConfigNode::setDate(time_t stamp)
{
    mDate = Date(stamp);
}