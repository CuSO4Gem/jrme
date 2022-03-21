#include <algorithm> 
#include <vector>

#include "JournalIOFactory.h"
#include "SfJournalBook.h"
#include "Utils.h"

struct fastSortData
{
    time_t stamp;
    size_t order;
};

void SfJournalBook::setKey(uint8_t key[32])
{
    ;
}

bool SfJournalBook::open(string path)
{
    //todo : add more formate of JournalIO
    mJournalIO = JournalIOFactory().getJournalIO(path);
    if (!mJournalIO->open(path))
        return false;
    
    mJournalIO->setReadMod();
    shared_ptr<Journal> journal;
    while ((journal=mJournalIO->readJournal()) != nullptr)
    {
        mJournalList.push_back(journal);
    }
    
    return true;
}

void SfJournalBook::close()
{
    AutoLock aLock = AutoLock(mJournalListLock);
    mJournalIO->close();
    mJournalList.clear();
}

static bool fastSortDataCmp(const fastSortData &d1,const fastSortData &d2)
{
    return d1.stamp<d2.stamp;
}

void SfJournalBook::order()
{
    //todo : more flexable sort
    AutoLock aLock = AutoLock(mJournalListLock);
    vector<fastSortData> fsdVector = vector<fastSortData>(mJournalList.size());
    size_t pos = 0;
    for (auto &it:mJournalList)
    {
        fsdVector[pos].order = pos;
        fsdVector[pos].stamp = getStampFormConfig(it->getConfig());
        pos++;
    }
    sort(fsdVector.begin(), fsdVector.end(), fastSortDataCmp);
    list<shared_ptr<Journal>> jTmpList = list<shared_ptr<Journal>>(mJournalList);
    list<shared_ptr<Journal>>::iterator jListIt = mJournalList.begin();
    for (pos=0; pos<fsdVector.size(); pos++)
    {
        list<shared_ptr<Journal>>::iterator jTmpIt = jTmpList.begin();
        for (size_t i = 0; i < fsdVector[pos].order; i++)
        {
            jTmpIt++;
        }
        *jListIt = *jTmpIt;

        // 最后一次不自增，防止野指针
        if (jListIt != mJournalList.end())
            jListIt++;
    }

    /*verfy*/
    
}

bool SfJournalBook::save()
{
    AutoLock aLock = AutoLock(mJournalListLock);
    if (!mJournalIO->setWriteMode())
        return false;

    for (auto &it:mJournalList)
    {
        if (!mJournalIO->writeJournal(it))
        {
            return false;
        }
    }
    return true;
}

size_t SfJournalBook::size()
{
    AutoLock aLock = AutoLock(mJournalListLock);
    return mJournalList.size();
}

shared_ptr<Journal> SfJournalBook::at(size_t pos)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    if (pos >= mJournalList.size())
        return nullptr;
    
    list<shared_ptr<Journal>>::iterator it = mJournalList.begin();
    for (size_t i = 0; i < pos; i++)
    {
        it++;
    }

    return *(it);
}

shared_ptr<Journal> SfJournalBook::operator [](size_t pos)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    if (pos >= mJournalList.size())
        return nullptr;

    list<shared_ptr<Journal>>::iterator it = mJournalList.begin();
    for (size_t i = 0; i < pos; i++)
    {
        it++;
    }

    return *(it);
}

bool SfJournalBook::insert(size_t pos, shared_ptr<Journal> journal)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    if (pos<0 || pos>mJournalList.size())
        return false;

    //todo : test
    if (pos==mJournalList.size())
    {
        mJournalList.push_back(journal);
        return true;
    }

    list<shared_ptr<Journal>>::iterator it = mJournalList.begin();
    for (size_t i = 0; i < pos; i++)
    {
        it++;
    }
    
    mJournalList.insert(it, journal);
    return true;
}

void SfJournalBook::push_front(shared_ptr<Journal> journal)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    mJournalList.push_front(journal);
}

void SfJournalBook::push_back(shared_ptr<Journal> journal)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    mJournalList.push_back(journal);
}

void SfJournalBook::erase(size_t pos)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    if (pos<0 || pos>=mJournalList.size())
    {
        return;
    }
    
    list<shared_ptr<Journal>>::iterator it = mJournalList.begin();
    for (size_t i = 0; i < pos; i++)
    {
        it++;
    }
    mJournalList.erase(it);
}

bool SfJournalBook::swap(size_t pos1, size_t pos2)
{
    AutoLock aLock = AutoLock(mJournalListLock);
    if (pos1<0 || pos1>=mJournalList.size()||
        pos2<0 || pos2>=mJournalList.size())
        return false;
    
    list<shared_ptr<Journal>>::iterator it1 = mJournalList.begin();
    for (size_t i = 0; i < pos1; i++)
    {
        it1++;
    }

    list<shared_ptr<Journal>>::iterator it2 = mJournalList.begin();
    for (size_t i = 0; i < pos2; i++)
    {
        it2++;
    }

    iter_swap(it1, it2);
    return true;
}
