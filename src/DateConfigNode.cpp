#include "DateConfigNode.h"
#include "utils.h"

DateConfigNode::DateConfigNode()
{
    mDate = Date(1900, 1, 1, 9, 0, 0);
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

string DateConfigNode::beforeInput(shared_ptr<Journal> journal)
{
    //todo : from timeparser
    setValueToConfig(journal->getConfig(), "date", mDate.toString());
}

int32_t DateConfigNode::innerConfigType()
{
    return DATE_CONFIG;
}

void DateConfigNode::setDate(time_t stamp)
{
    mDate = Date(stamp);
}