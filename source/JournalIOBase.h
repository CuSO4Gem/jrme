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
#ifndef JOURNAL_IO_BASE_H
#define JOURNAL_IO_BASE_H

#include <memory>
#include <string>
#include <vector>

#include "Journal.h"

using std::shared_ptr;
using std::string;
using std::vector;

/**
 * @brief
 * 用于日记IO的接口类型，为了实现不同格式的日记保存工作
 *
 * Interface class for IO journal. To support different formate of journal.
 *
 */
class JournalIOBase
{
public:
    virtual ~JournalIOBase() = default;

    virtual uint32_t apiSupport() = 0;

    /**
     * @brief 日记接口所支持的格式后缀
     * format suffix supported by JournalIOBase
     *
     * @return vector<string> formate list
     */
    virtual vector<string> formateSupport() = 0;

    virtual bool isSupportAes256()       = 0;
    virtual void setKey(uint8_t key[32]) = 0;
    virtual void clearKey()              = 0;

    /**
     * @brief 打开一个文件，准备解析。
     * open file and ready to parse
     *
     * @param[in] path 文件的路径。the journal path
     * @return true open success
     * @return false open failed
     */
    virtual bool open(string path) = 0;

    /**
     * @brief 关闭文件
     * close file
     */
    virtual void close() = 0;

    /**
     * @brief 设置为读取模式
     * set to read mode
     *
     * @details 读取模式，JrounalIO应该可以依次读取每一篇日记，且不能进行写入操作。
     * In read mode, JournalIO should be able to read each journal one by one, and can't write.
     *
     * @return true 设置成功。set success
     * @return false 设置失败。set failed
     */
    virtual bool setReadMod() = 0;

    /**
     * @brief 设置为写入模式。
     * set to write mode
     *
     * @details 写入模式，JournalIO应该从0开始，依次写入每一篇日记，且不能进行读取操作。
     * In write mode, JournalIO should start from 0 offset,
     * and write each journal one by one, and can't read.
     *
     * @return true 设置成功。set success
     * @return false 设置失败。set failed
     */
    virtual bool setWriteMode() = 0;

    /**
     * @brief 读取一篇日记
     * read one journal
     *
     * @return 日记的指针。journal pointer
     *
     * @retval nullptr 读取失败，或者以及读取到文件末尾了。read failed, or reach end of file.
     * @return not nllptr 读取成功。read success
     */
    virtual shared_ptr<Journal> readJournal() = 0;

    /**
     * @brief 写入一篇日记
     * write one journal
     *
     * @return true
     * @return false
     */
    virtual bool writeJournal(shared_ptr<Journal> journal) = 0;
};
#endif