#ifndef TIME_PARSER_BASE_H
#define TIME_PARSER_BASE_H

#include <string>

#include "timeParserType.h"

using namespace std;

class TimeParserBase
{
public:
    ~TimeParserBase() = default;

    virtual timeParserRet parse(string description) = 0;

    virtual bool loadPlugin(string path) {return true;};
};

#endif