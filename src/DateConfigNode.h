#ifndef DATE_CONFIG_NODE_H
#define DATE_CONFIG_NODE_H

#include "ConfigNodeBase.h"
#include "date.h"
#include "utils.h"

using namespace ec;

class DateConfigNode : public ConfigNodeBase
{
    Date mDate;
public:
    DateConfigNode() {mDate = Time().toDate();};
    ~DateConfigNode() = default;

    uint32_t apiVersion() {return 1;};

    string getKey() {return string("date");};
    string getDefaultValue() {return string("");};
    void beforeInput(shared_ptr<Journal> journal) {setValueToConfig(journal->getConfig(), "date", mDate.toString());};
    void afterInput(shared_ptr<Journal> journal) {return;};

    int32_t innerConfigType() {return DATE_CONFIG;};
    void setDate(time_t stamp) {mDate = Date(stamp);};
};

#endif