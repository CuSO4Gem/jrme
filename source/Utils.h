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
#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <string>
#include <vector>

#include "Journal.h"
#include "JournalBookBase.h"
#include "date.h"

using namespace ::std;
using namespace ::ec;

inline bool isNumChar(const char ch)
{
    return '0' <= ch && ch <= '9';
}

/**
 * @brief
 * 移除连续的多个空格
 * remove multiple space from string
 */
void removeMultipleSpaces(string &str);

/**
 * @brief
 * 从日记的attributePart部分获取指定key的value
 * Given key return value from journal attributePart
 *
 * @param attributePart
 * @param key
 * @param value
 * @return true find
 * @return false not find
 */
bool getValueFromJAttributePart(const string &attributePart, const string &key, string &value);

/**
 * @brief
 * 向日记的attributePart部分设置指定key的value
 * Set the Value To attributePart
 *
 * @param attributePart
 * @param key
 * @param value
 * @return true
 * @return false
 */
bool setValueToJAttributePart(string &attributePart, const string &key, const string value);

/**
 * @brief
 * 从attributePart部分获取时戳
 * Get the Stamp Form attributePart string
 *
 * @param attributePart
 * @return time_t
 */
time_t getStampFormJAttributePart(const string &attributePart);

/**
 * @brief
 * 从attributePart部分获取level值
 * Get the Level Form Journal attributePart
 *
 * @param attributePart
 * @return int32_t
 */
int32_t getLevelFormJAttributePart(const string &attributePart);

/**
 * @brief
 * 从attributePart解析tag的vector
 * Get the Tags Form Journal attributePart
 */
vector<string> getTagsFormJAttributePart(const string &attributePart);

/**
 * @brief
 * 将字符串转换为日记（只能txt格式）
 * Get string to journal (txt formate)
 */
shared_ptr<Journal> strToJournal(const string &inStr);

/**
 * @brief
 * 转移路径除开头的空格，将~转换为绝对路径
 *
 * conver rawPath to a valid path
 * remove spaces at begin, conver ~ to home dir
 */
string validPath(string rawPath);

/**
 * @brief
 * 要求用户输入一个路径
 * Let user input a file path
 */
string getFilePath();

/**
 * @brief
 * 自动创建一个日记本类型
 * select journal book by path
 *
 * @param path
 * @return shared_ptr<JournalBookBase>
 */
shared_ptr<JournalBookBase> bookFactory(string path);
#endif