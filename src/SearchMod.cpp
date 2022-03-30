#include <iomanip>
#include <sstream>

#include "date.h"
#include "debug_print.h"
#include "EnTimeParser.h"
#include "JournalFilter.h"
#include "SearchMod.h"
#include "Utils.h"

using namespace ec;

Date dataBegin(timeParserRet timeRet)
{
    Date date;
    date.setYear(timeRet.flag&YEAR_FLAG ? timeRet.year : 1970);
    date.setMonth(timeRet.flag&MONTH_FLGA ? timeRet.month : 1);
    date.setDay(timeRet.flag&DAY_FLAG ? timeRet.day : 1);
    date.setHour(timeRet.flag&HOUR_FLAG ? timeRet.hour : 0);
    date.setMinute(timeRet.flag&MINUTE_FLAG ? timeRet.minute : 0);
    date.setSecond(timeRet.flag&SECOND_FLAG ? timeRet.second : 0);

    return date;
}

Date dataEnd(timeParserRet timeRet)
{
    Date now = Time().toDate();
    Date date;
    date.setYear(timeRet.flag&YEAR_FLAG ? timeRet.year : now.year());
    date.setMonth(timeRet.flag&MONTH_FLGA ? timeRet.month : 12);
    if (timeRet.flag%DAY_FLAG)
        date.setDay(timeRet.day);
    else
    {
        int data = Date::yearMonthDays(date.year(), date.month());
        date.setDay(data);
    }
    date.setHour(timeRet.flag&HOUR_FLAG ? timeRet.hour : 23);
    date.setMinute(timeRet.flag&MINUTE_FLAG ? timeRet.minute : 59);
    date.setSecond(timeRet.flag&SECOND_FLAG ? timeRet.second : 59);
    
    return date;
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
        map<string, size_t> allTags = filter.tagsCount();
        size_t firstLen=0;
        for (auto &it:allTags)
        {
            if (firstLen<it.first.length())
                firstLen = it.first.length();
        }
        firstLen += 4;

        for (auto &it:allTags)
        {
            cout << setw(firstLen) << it.first;
            printf("%ld\n", it.second);
        }
        return 0;
    }

    if (cmd.exist("level"))
    {
        string value = cmd.get<string>("level");
        stringstream valueStream = stringstream(value);
        string buffer;
        size_t inputNumber = 0;
        int32_t min, max;
        while (getline(valueStream, buffer, '~'))
        {
            if (inputNumber==0)
                min = atoi(buffer.c_str());
            if (inputNumber==1)
                max = atoi(buffer.c_str());
            else
                JLOGW("too many input in --level"); 
            inputNumber++;
        }
        if (min>max)
        {
            int32_t t = min;
            min = max;
            max = t;
        }

        if (inputNumber==1)
        {
            filter.levelFilter(min, false);
            filter.levelFilter(min, true);
        }
        else if (inputNumber==2)
        {
            filter.levelFilter(min, false);
            filter.levelFilter(max, true);
        }
    }

    if (cmd.exist("on"))
    {
        string value = cmd.get<string>("on");
        if (value.length()==0)
        {
            printf("value of --on error");
            return -1;
        }
        EnTimeParser parser;
        timeParserRet timeRet = parser.parse(value);
        if (timeRet.estimation<TIME_PARSE_LITTLE_SUCCESS)
        {
            JLOGE("EnTimeParser faild");
            printf("cannot parser \"%s\"", value.c_str());
        }
        
        Date date = dataBegin(timeRet);
        filter.stampFilter(date.stamp(), false);
        date = dataEnd(timeRet);
        filter.stampFilter(date.stamp(), true);
        filter.sortByStamp(); 
    }

    if ((cmd.exist("from") || cmd.exist("to")) && !cmd.exist("on"))
    {
        bool acsSort = false;
        if (cmd.exist("from"))
        {
            acsSort = true;
            string value = cmd.get<string>("from");
            if (value.length()==0)
            {
                printf("value of --from error");
                return -1;
            }
            EnTimeParser parser;
            timeParserRet timeRet = parser.parse(value);
            Date date;
            date.setYear(timeRet.year);
            date.setMonth(timeRet.month);
            date.setDay(timeRet.day);
            date.setHour(timeRet.hour);
            date.setMinute(timeRet.minute);
            date.setSecond(timeRet.second);
            filter.stampFilter(date.stamp(), false);
        }
        if (cmd.exist("to"))
        {
            string value = cmd.get<string>("to");
            if (value.length()==0)
            {
                printf("value of --to error");
                return -1;
            }
            EnTimeParser parser;
            timeParserRet timeRet = parser.parse(value);
            Date date;
            date.setYear(timeRet.year);
            date.setMonth(timeRet.month);
            date.setDay(timeRet.day);
            date.setHour(timeRet.hour);
            date.setMinute(timeRet.minute);
            date.setSecond(timeRet.second);
            filter.stampFilter(date.stamp(), true);
        }
        filter.sortByStamp(acsSort);   
    }

    if (cmd.exist("tags"))
    {
        string tags = cmd.get<string>("tags");
        if (tags.length() == 9)
        {
            printf("value of --tags error");
            return -1;
        }
        stringstream tagsStream = stringstream(tags);
        vector<string> tagsVector;
        string tagBuffer;
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

    /*************print*************/
    vector<size_t> orderVector = filter.getJournalOrder();
    for (auto &it:orderVector)
    {
        printf("%s\n",book->at(it)->toString().c_str());
    }

    return 0;
}