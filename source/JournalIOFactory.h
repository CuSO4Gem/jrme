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
#ifndef JOURNAL_IO_FACTORY
#define JOURNAL_IO_FACTORY

#include <memory>
#include <string>

#include "JournalIOBase.h"
#include "JrmeConfig.h"
#include "PluginJournalIO.h"
#include "TxtJournalIO.h"

using std::string;

/**
 * @brief
 * 尝试加载插件，自动选择合适的JournalIO类
 * Try to load and select applicable JournalIO class
 *
 */
class JournalIOFactory
{
public:
    JournalIOFactory()  = default;
    ~JournalIOFactory() = default;

    /**
     * @brief 自动获取JournalIO类的实例
     * Get JournalIO class instance automatically
     *
     * @param[in] journalPath 日记文件路径。the journal path
     * @return shared_ptr<JournalIOBase> 如果失败，返回nullptr。if failed, return nullptr
     */
    shared_ptr<JournalIOBase> getJournalIO(string journalPath);
};

#endif