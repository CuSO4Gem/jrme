#ifndef JOURNAL_FILTER_H
#define JOURNAL_FILTER_H

#include <map>
#include <string>
#include <vector>

#include "Journal.h"
#include "JournalBookBase.h"
#include "memory"

using namespace std;

typedef struct guidance_s
{
    size_t order; //order in book
    time_t stamp;
    int32_t level;
    vector<string> tags;
} guidance;


class JournalFilter
{
private:
    shared_ptr<JournalBookBase> mBook;
    vector <guidance> mGuidance;
    uint32_t mCacheFlag;

    void tryCache(uint32_t cacheFlag);

    static bool orderAscCmp(const guidance &d1,const guidance &d2);
    static bool orderDescCmp(const guidance &d1,const guidance &d2);
    static bool stampAscCmp(const guidance &d1,const guidance &d2);
    static bool stampDescCmp(const guidance &d1,const guidance &d2);
    static bool levelAscCmp(const guidance &d1,const guidance &d2);
    static bool levelDescCmp(const guidance &d1,const guidance &d2);

public:
    JournalFilter(shared_ptr<JournalBookBase> book);
    ~JournalFilter() = default;

    vector<size_t> getJournalOrder();

    void sortByOrder(bool asc = true);
    void sortByStamp(bool asc = true);
    void sortByLevel(bool asc = true);

    void stampFilter(time_t stamp, bool uperrLimit);
    void levelFilter(int32_t level, bool uperrLimit);
    void withTagsFilter(vector<string> tags);

    map<string, size_t> tagsCount();
};

#endif