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
#include <iomanip>
#include <sstream>

#include "EnTimeParser.h"
#include "JournalFilter.h"
#include "SearchMod.h"
#include "TxtEditor.h"
#include "Utils.h"
#include "date.h"
#include "debug_print.h"

using ec::Date;

#define WARNING_PERCENTAGE (20)

/**
 * @brief
 * 给定一个timeParserRet结构图，对于未确定的时间，尽量早。
 * Given a timeParserRet struct, return a Date as early as posable
 */
Date dataBegin(timeParserRet timeRet)
{
    Date date;
    date.setYear(timeRet.flag & YEAR_FLAG ? timeRet.year : 1970);
    date.setMonth(timeRet.flag & MONTH_FLGA ? timeRet.month : 1);
    date.setDay(timeRet.flag & DAY_FLAG ? timeRet.day : 1);
    date.setHour(timeRet.flag & HOUR_FLAG ? timeRet.hour : 0);
    date.setMinute(timeRet.flag & MINUTE_FLAG ? timeRet.minute : 0);
    date.setSecond(timeRet.flag & SECOND_FLAG ? timeRet.second : 0);

    return date;
}

/**
 * @brief
 * 给定一个timeParserRet结构图，对于未确定的时间，尽量量晚。
 * Given a timeParserRet struct, return a Date as late as posable
 */
Date dataEnd(timeParserRet timeRet)
{
    Date now = Time().toDate();
    Date date;
    date.setYear(timeRet.flag & YEAR_FLAG ? timeRet.year : now.year());
    date.setMonth(timeRet.flag & MONTH_FLGA ? timeRet.month : 12);
    if (timeRet.flag % DAY_FLAG)
        date.setDay(timeRet.day);
    else
    {
        int data = Date::yearMonthDays(date.year(), date.month());
        date.setDay(data);
    }
    date.setHour(timeRet.flag & HOUR_FLAG ? timeRet.hour : 23);
    date.setMinute(timeRet.flag & MINUTE_FLAG ? timeRet.minute : 59);
    date.setSecond(timeRet.flag & SECOND_FLAG ? timeRet.second : 59);

    return date;
}

void editJournal(shared_ptr<JournalBookBase> book, size_t order)
{
    if (order < 0 || order > book->size())
    {
        JLOGE("[E] edit error book size is %d, order is %d", book->size(), order);
        return;
    }
    TxtEditor editor = TxtEditor();
    editor.setInitStr(book->at(order)->toString());
    shared_ptr<Journal> journal = editor.getJournalFromEditor();
    book->erase(order);
    book->insert(order, journal);
}

int journalSearchMod(cmdline::parser &cmd, string bookPath)
{
    shared_ptr<JournalBookBase> book = bookFactory(bookPath);
    if (!book->open(bookPath))
    {
        printf("cannot open journal book %s\n", bookPath.c_str());
        return -1;
    }
    JournalFilter filter = JournalFilter(book);

    if (cmd.exist("all_tags"))
    {
        map<string, size_t> allTags  = filter.tagsCount();
        size_t              firstLen = 0;
        JLOGI("[I] Search all_tags, get %ld tags", allTags.size());
        for (auto &it : allTags)
        {
            if (firstLen < it.first.length())
                firstLen = it.first.length();
        }

        for (auto &it : allTags)
        {
            cout << setw(firstLen) << it.first;
            printf("\t%ld\n", it.second);
        }
        return 0;
    }

    if (cmd.exist("level"))
    {
        string       value       = cmd.get<string>("level");
        stringstream valueStream = stringstream(value);
        string       buffer;
        size_t       inputNumber = 0;
        int32_t      min, max;
        while (getline(valueStream, buffer, '~'))
        {
            if (inputNumber == 0)
                min = atoi(buffer.c_str());
            else if (inputNumber == 1)
                max = atoi(buffer.c_str());
            else
                JLOGW("[W] too many input in --level");
            inputNumber++;
        }
        /*when input 2 numbers, ensure min<max*/
        if (inputNumber >= 2 && min > max)
        {
            int32_t t = min;
            min       = max;
            max       = t;
        }

        if (inputNumber == 1)
        {
            JLOGI("[I] Search level equal %d", min);
            filter.levelFilter(min, false);
            filter.levelFilter(min, true);
        }
        else if (inputNumber == 2)
        {
            JLOGI("[I] Search level %d to %d", min, max);
            filter.levelFilter(min, false);
            filter.levelFilter(max, true);
        }
    }

    if (cmd.exist("on"))
    {
        string value = cmd.get<string>("on");
        if (value.length() == 0)
        {
            printf("value of --on error");
            return -1;
        }
        EnTimeParser  parser;
        timeParserRet timeRet = parser.parse(value);
        if (timeRet.estimation < TIME_PARSE_LITTLE_SUCCESS || timeRet.flag == 0)
        {
            JLOGE("[E] EnTimeParser faild");
            printf("cannot parser \"%s\"", value.c_str());
            return -1;
        }

        Date date = dataBegin(timeRet);
        filter.stampFilter(date.stamp(), false);
        JLOGI("[I] Search on, from %s", date.toString().c_str());
        date = dataEnd(timeRet);
        filter.stampFilter(date.stamp(), true);
        JLOGI("[I] Search on, to %s", date.toString().c_str());
    }

    if ((cmd.exist("from") || cmd.exist("to")) && !cmd.exist("on"))
    {
        if (cmd.exist("from"))
        {
            string value = cmd.get<string>("from");
            if (value.length() == 0)
            {
                printf("value of --from error");
                return -1;
            }
            EnTimeParser  parser;
            timeParserRet timeRet = parser.parse(value);
            if (timeRet.estimation < TIME_PARSE_LITTLE_SUCCESS || timeRet.flag == 0)
            {
                JLOGE("[E] EnTimeParser faild");
                printf("cannot parser \"%s\"", value.c_str());
                return -1;
            }
            Date date = dataBegin(timeRet);
            JLOGI("[I] Search from the date:%s", date.toString().c_str());
            filter.stampFilter(date.stamp(), false);
        }
        if (cmd.exist("to"))
        {
            string value = cmd.get<string>("to");
            if (value.length() == 0)
            {
                printf("value of --to error");
                return -1;
            }
            EnTimeParser  parser;
            timeParserRet timeRet = parser.parse(value);
            if (timeRet.estimation < TIME_PARSE_LITTLE_SUCCESS || timeRet.flag == 0)
            {
                JLOGE("[E] EnTimeParser faild");
                printf("cannot parser \"%s\"", value.c_str());
                return -1;
            }
            Date date = dataEnd(timeRet);
            JLOGI("[I] Search to the date:%s", date.toString().c_str());
            filter.stampFilter(date.stamp(), true);
        }
    }

    if (cmd.exist("tags"))
    {
        string tags = cmd.get<string>("tags");
        if (tags.length() == 0)
        {
            printf("value of --tags error");
            return -1;
        }
        stringstream   tagsStream = stringstream(tags);
        vector<string> tagsVector;
        string         tagBuffer;
        tagsVector.reserve(20);
        while (getline(tagsStream, tagBuffer))
        {
            tagsVector.push_back(tagBuffer);
        }
        filter.withTagsFilter(tagsVector);
    }

    /*************sort*************/
    if (cmd.exist("on") || cmd.exist("from"))
        filter.sortByStamp(true);
    else if (cmd.exist("to"))
        filter.sortByStamp(false);
    else if (cmd.exist("level"))
        filter.sortByLevel();

    vector<size_t> orderVector = filter.getJournalOrder();
    if (cmd.exist("number"))
    {
        size_t number = cmd.get<size_t>("number");
        while (number >= 0 && orderVector.size() > number)
        {
            orderVector.pop_back();
        }
    }

    /*************editor*************/
    if (cmd.exist("edit"))
    {
        if (orderVector.size() == 0)
            return 0;
        if (orderVector.size() == 1)
        {
            editJournal(book, orderVector[0]);
            book->save();
            return 0;
        }
        else if (orderVector.size() > 1)
        {
            bool   sure = true;
            string gotAnswer;
            printf("find %ld journal, are you sure to editor them one by one?(Y/N)", orderVector.size());
            getline(cin, gotAnswer);
            if (!(gotAnswer[0] == 'Y' || gotAnswer[0] == 'y'))
                sure = false;

            /*editor one journal*/
            if (!sure)
            {
                editJournal(book, orderVector[0]);
                book->save();
                return 0;
            }

            /*editor journal one by one*/
            for (auto &it : orderVector)
            {
                editJournal(book, it);
            }
            book->save();
            return 0;
        }
        else
        {
            JLOGE("[E] some thing error in filter");
            return -1;
        }
    }

    /*************print/delete*************/
    if ((cmd.exist("delete") || cmd.exist("force_delete")) && !cmd.exist("edit"))
    {
        JLOGI("delete %ld journal", orderVector.size());
        if (orderVector.size() == 0)
        {
            printf("nothing to delete\n");
            return 0;
        }
        size_t percent = 100 * orderVector.size() / book->size();
        if (percent > WARNING_PERCENTAGE)
        {
            JLOGW("delete joural for %ld%%", percent);
        }

        bool sure = true;
        if (cmd.exist("force_delete"))
            sure = true;
        else
        {
            string gotAnswer;
            if (orderVector.size() == 0)
                printf("Are you sure to delete a journals?(Y/N)");
            else
                printf("Are you sure to delete %ld journals?(Y/N)", orderVector.size());
            getline(cin, gotAnswer);
            if (!(gotAnswer[0] == 'Y' || gotAnswer[0] == 'y'))
                sure = false;
        }
        if (!sure)
            return 0;

        // resort to delete
        filter.sortByOrder();
        orderVector = filter.getJournalOrder();
        size_t i    = 0;
        while (i < orderVector.size())
        {
            orderVector[i] -= i;
            if (orderVector[i] < 0)
            {
                JLOGE("[E] something error while delete");
                printf("nothing to delete for someting error\n");
                return -1;
            }
            i++;
        }
        for (auto &it : orderVector)
        {
            book->erase(it);
        }
        book->save();
    }
    else
    {
        for (auto &it : orderVector)
        {
            printf("%s\n", book->at(it)->toString().c_str());
        }
    }

    return 0;
}