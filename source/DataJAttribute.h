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
#ifndef DATE_JATTRIBUTE_H
#define DATE_JATTRIBUTE_H

#include "JAttributeBase.h"
#include "Utils.h"
#include "date.h"

using ec::Date;

/**
 * @brief
 * 处理"date"JAttribute，用于记录日记的时间
 *
 * Process "date" JAttribute. Mark the date of journal
 */
class DataJAttribute : public JAttributeBase
{
    Date mDate;

public:
    DataJAttribute()
    {
        mDate = Time().toDate();
    };
    ~DataJAttribute() = default;

    uint32_t apiVersion()
    {
        return 1;
    };

    string getKey()
    {
        return string("date");
    };
    string getDefaultValue()
    {
        return string("");
    };
    void preprocess(shared_ptr<Journal> journal)
    {
        setValueToJAttributePart(journal->getAttributePart(), "date", mDate.toString());
    };
    void postprocess(shared_ptr<Journal> journal)
    {
        return;
    };

    int32_t getInnerJAttributeType()
    {
        return DATE_JATTRIBUTE;
    };
    void setDate(time_t stamp)
    {
        mDate = Date(stamp);
    };
};

#endif