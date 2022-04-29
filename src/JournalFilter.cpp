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
#include <algorithm> 
#include <set>

#include "debug_print.h"
#include "JournalFilter.h"
#include "Utils.h"

#define CACHE_STAMP (0x1)
#define CACHE_LEVEL (0x1 << 1)
#define CACHE_TAGES  (0x1 << 2)

JournalFilter::JournalFilter(shared_ptr<JournalBookBase> book)
{
    if (!book)
        return;
    mBook = book;
    mCacheFlag = 0;
    mGuidance.resize(mBook->size());

    size_t i=0;
    for (auto &it:mGuidance)
    {
        it.order = i;
        i++;
    }
}

void JournalFilter::tryCache(uint32_t cacheFlag)
{
    if (!mBook)
        return;
    cacheFlag &= ~mCacheFlag;
    if (cacheFlag&CACHE_STAMP)
    {
        for (auto &it:mGuidance)
        {
            shared_ptr<Journal> journal = mBook->at(it.order);
            it.stamp = getStampFormConfig(journal->getConfig());
        }
        mCacheFlag |= CACHE_STAMP;
    }

    if (cacheFlag&CACHE_LEVEL)
    {
        for (auto &it:mGuidance)
        {
            shared_ptr<Journal> journal = mBook->at(it.order);
            it.level = getLevelFormConfig(journal->getConfig());
        }
        mCacheFlag |= CACHE_LEVEL;
    }

    if (cacheFlag&CACHE_TAGES)
    {
        for (auto &it:mGuidance)
        {
            shared_ptr<Journal> journal = mBook->at(it.order);
            it.tags = getTagsFormConfig(journal->getConfig());
        }
        mCacheFlag |= CACHE_TAGES;
    }
}

vector<size_t> JournalFilter::getJournalOrder()
{
    vector<size_t> orderVector(mGuidance.size());
    for (size_t i = 0; i < orderVector.size(); i++)
    {
        orderVector[i] = mGuidance[i].order;
    }
    return orderVector;
}

bool JournalFilter::orderAscCmp(const guidance &d1,const guidance &d2)
{
    return d1.order<d2.order;
}

bool JournalFilter::orderDescCmp(const guidance &d1,const guidance &d2)
{
    return d1.order>d2.order;
}

bool JournalFilter::stampAscCmp(const guidance &d1,const guidance &d2)
{
    return d1.stamp<d2.stamp;
}

bool JournalFilter::stampDescCmp(const guidance &d1,const guidance &d2)
{
    return d1.stamp>d2.stamp;
}

bool JournalFilter::levelAscCmp(const guidance &d1,const guidance &d2)
{
    return d1.level<d2.level;
}

bool JournalFilter::levelDescCmp(const guidance &d1,const guidance &d2)
{
    return d1.level>d2.level;
}


void JournalFilter::sortByOrder(bool asc) {
    if (!mBook || mGuidance.size()==0)
        return;
    if (asc)
        sort(mGuidance.begin(), mGuidance.end(), orderAscCmp);
    else 
        sort(mGuidance.begin(), mGuidance.end(), orderDescCmp);
}

void JournalFilter::sortByStamp(bool asc)
{
    if (!mBook || mGuidance.size()==0)
        return;
    tryCache(CACHE_STAMP);
    if (asc)
        sort(mGuidance.begin(), mGuidance.end(), stampAscCmp);
    else 
        sort(mGuidance.begin(), mGuidance.end(), stampDescCmp);
}

void JournalFilter::sortByLevel(bool asc)
{
     if (!mBook || mGuidance.size()==0)
        return;
    tryCache(CACHE_LEVEL);
    if (asc)
        sort(mGuidance.begin(), mGuidance.end(), levelAscCmp);
    else 
        sort(mGuidance.begin(), mGuidance.end(), levelDescCmp);
}

void JournalFilter::stampFilter(time_t stamp, bool uperrLimit)
{
    tryCache(CACHE_STAMP);
    if (uperrLimit)
    {
        size_t i=0;
        while (i<mGuidance.size())
        {
            if (mGuidance[i].stamp>stamp)
                mGuidance.erase(mGuidance.begin()+i);
            else
                i++;
        }
    }
    else
    {
        size_t i=0;
        while (i<mGuidance.size())
        {
            if (mGuidance[i].stamp<stamp)
                mGuidance.erase(mGuidance.begin()+i);
            else
                i++;
        }
    }
}

void JournalFilter::levelFilter(int32_t level, bool uperrLimit)
{
    tryCache(CACHE_LEVEL);
    if (uperrLimit)
    {
        size_t i=0;
        while (i<mGuidance.size())
        {
            if (mGuidance[i].level>level)
                mGuidance.erase(mGuidance.begin()+i);
            else
                i++;
        }
    }
    else
    {
        size_t i=0;
        while (i<mGuidance.size())
        {
            if (mGuidance[i].level<level)
                mGuidance.erase(mGuidance.begin()+i);
            else
                i++;
        }
    }
}

void JournalFilter::withTagsFilter(const vector<string> &tags)
{
    tryCache(CACHE_TAGES);
    size_t i=0;
    set<string> tagsSet = set<string>(tags.begin(), tags.end());
    
    while (i<mGuidance.size())
    {
        bool find = false;
        for (auto &it:mGuidance[i].tags)
        {
            if (tagsSet.find(it)!=tagsSet.end())
            {
                find = true;
                break;
            }
        }
        if (find)
            i++;
        else
            mGuidance.erase(mGuidance.begin()+i);
    }
}

map<string, size_t> JournalFilter::tagsCount()
{
    tryCache(CACHE_TAGES);
    map<string, size_t> tagsMap = map<string, size_t>();

    for (size_t pos = 0; pos < mGuidance.size(); pos++)
    {
        for (auto &it:mGuidance[pos].tags)
        {
            map<string, size_t>::iterator mapIt = tagsMap.find(it);
            if (mapIt == tagsMap.end())
            {
                tagsMap.insert(pair<string, size_t>(it, 1));
            }
            else
            {
                mapIt->second += 1;
            }
        }
    }
    return tagsMap;
}
