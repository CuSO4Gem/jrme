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
#ifndef JOURNAL_FILTER_H
#define JOURNAL_FILTER_H

#include <map>
#include <string>
#include <vector>

#include "Journal.h"
#include "JournalBookBase.h"
#include "memory"

using namespace std;

/**
 * @brief 日记过滤时需要的缓存
 * a cache for journal filter
 *
 * @details
 * 缓存日记的stamp和level以及tags，用于加快排序和筛选的速度。
 * 相关信息不会在创建filter全部缓存，而是在进行相关排序和筛选的时候临时处理。
 * 该结构体为日记相关信息的缓存，日记本中每一篇日记对应一个。
 *
 * To cache stamp, level and tags of journal, in order to bost sort and filter.
 * Data will not caceh everything while filter construct. It will be cache before
 * filter and sort.
 * This structure is a cache of journal's stamp, level and tags, and each journal in journal book.
 */
typedef struct guidance_s
{
    size_t         order; /*!< 日记的排序序号 */
    time_t         stamp; /*!< 日记的时间戳 */
    int32_t        level; /*!< 日记的等级 */
    vector<string> tags;  /*!< 日记的标签 */
} guidance;

class JournalFilter
{
  private:
    shared_ptr<JournalBookBase> mBook;
    vector<guidance>            mGuidance;
    uint32_t                    mCacheFlag;

    void tryCache(uint32_t cacheFlag);

    static bool orderAscCmp(const guidance &d1, const guidance &d2);
    static bool orderDescCmp(const guidance &d1, const guidance &d2);
    static bool stampAscCmp(const guidance &d1, const guidance &d2);
    static bool stampDescCmp(const guidance &d1, const guidance &d2);
    static bool levelAscCmp(const guidance &d1, const guidance &d2);
    static bool levelDescCmp(const guidance &d1, const guidance &d2);

  public:
    JournalFilter(shared_ptr<JournalBookBase> book);
    ~JournalFilter() = default;

    /**
     * @brief 获取Filter中剩余日记的编号
     * Get the order number of journal in filter
     *
     * @return vector<size_t>
     */
    vector<size_t> getJournalOrder();

    /**
     * @brief 按照编号排序。该编号为读取日记时的顺序。
     * Order by journal order number. The order number is the order of reading journal.
     * @param[in] asc 是否为升序排序. true:升序，false:降序。
     * if true, sort in ascending order. if false, sort in descending order.
     */
    void sortByOrder(bool asc = true);

    /**
     * @brief 按照日记的时戳排序
     * Order by journal stamp.
     * @param[in] asc  是否升序。true:升序；false:降序。
     * Order by stamp ascending. true: ascending; false: descending.
     */
    void sortByStamp(bool asc = true);

    /**
     * @brief 按照日记的Level排序
     * Order by journal level.
     * @param[in] asc  是否升序。true:升序；false:降序。
     * Order by level ascending. true: ascending; false: descending.
     */
    void sortByLevel(bool asc = true);

    /**
     * @brief 根据时戳过滤日记。等于该时戳的不会被滤除。
     * Filter journal by stamp. Journal with stamp equal to stamp will not be filtered.
     *
     * @param[in] stamp 时戳。time stamp.
     * @param[in] uperrLimit stamp是上限还是下限制。true:上限；false:下限。
     * stamp is upper limmit or lower limmit.if true, stamp is upper limit. if false, stamp is lower limit.
     */
    void stampFilter(time_t stamp, bool uperrLimit);

    /**
     * @brief 根据level滤除日记。等于该level的不会被滤除。
     *
     * @param[in] level
     * @param[in] uperrLimit level是上限还是下限制。true:上限；false:下限。
     * level is upper limmit or lower limmit.if true, level is upper limit. if false, level is lower limit.
     */
    void levelFilter(int32_t level, bool uperrLimit);

    /**
     * @brief 保留指定tag的日记。
     * Retain journal with specified tag.
     *
     * @param[in] tags
     */
    void withTagsFilter(const vector<string> &tags);

    /**
     * @brief 日记本中的tages以及数量。
     * Tags and count in journal book.
     * @details 注意，结果是包含tag的数量，不是日记的数量。一个日记中的多个tag，多次计算
     * Note: result is count of tag, not count of journal.
     * One journal has multiple tags, multiple times calculate.
     * @return map<string,int> tag and count.
     */
    map<string, size_t> tagsCount();
};

#endif