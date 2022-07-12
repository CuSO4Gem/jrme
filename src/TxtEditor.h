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
#ifndef TXT_EDITOR_H
#define TXT_EDITOR_H
#include <string>
#include <memory>

#include "Journal.h"

using namespace std;

class TxtEditor
{
private:
    string mInitStr;

public:
    TxtEditor() = default;
    TxtEditor(string initStr);
    ~TxtEditor() = default;

    /**
     * @brief 
     * 设置editor初始的字符串
     * Set init string of editor
     * 
     * @param[in] initStr 
     */
    void setInitStr(string initStr);

    /**
     * @brief 
     * 清除editor初始的字符串
     * Clear init string of editr
     */
    void clearInitStr();

    /**
     * @brief 
     * 让用户从editor输入日记
     * Get journal from user
     * 
     * @return nullptr 用户不希望保存
     */
    shared_ptr<Journal> getJournalFromEditor();
};

#endif