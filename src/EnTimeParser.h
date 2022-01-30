#ifndef EN_TIME_PARSER_H
#define EN_TIME_PARSER_H

#include "TimeParserBase.h"

class EnTimeParser : public TimeParserBase
{
private:
    /* data */
public:
    EnTimeParser() = default;
    ~EnTimeParser() = default;

    timeParserRet parse(string description);
};

#endif