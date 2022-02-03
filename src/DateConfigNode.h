#ifndef DATE_CONFIG_NODE_H
#define DATE_CONFIG_NODE_H

#include "ConfigNodeBase.h"
#include "date.h"

using namespace ec;

class DateConfigNode : public ConfigNodeBase
{
    Date mDate;
public:
    DateConfigNode();
    ~DateConfigNode() = default;

    uint32_t apiVersion();

    string getKey();
    string getDefaultValue();
    void beforeInput(shared_ptr<Journal> journal);
    void afterInput(shared_ptr<Journal> journal) {};

    int32_t innerConfigType();
    void setDate(time_t stamp);
};

#endif