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
#ifndef EN_TIME_PARSER_H
#define EN_TIME_PARSER_H

#include "TimeParserBase.h"

/**
 * @brief
 * 将英语的时间描述转换为具体的时间
 * Transfert english sentance to date.
 */
class EnTimeParser : public TimeParserBase
{
private:
    /* data */
public:
    EnTimeParser()  = default;
    ~EnTimeParser() = default;

    timeParserRet parse(string description);
};

#endif