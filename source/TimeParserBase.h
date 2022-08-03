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
#ifndef TIME_PARSER_BASE_H
#define TIME_PARSER_BASE_H

#include <string>

#include "timeParserType.h"

using std::string;

class TimeParserBase
{
public:
    ~TimeParserBase() = default;

    virtual timeParserRet parse(string description) = 0;

    virtual bool loadPlugin(string path) {return true;};
};

#endif