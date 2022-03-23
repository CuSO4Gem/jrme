#include <algorithm> 
#include <vector>

#include "debug_print.h"
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
    mJournalIO = JournalIOFactory().getJournalIO(path);
    if (!mJournalIO)
    {
        JLOGW("Cann't create journalIO");
        return false;
    }
    if (!mJournalIO->open(path))
    {
        JLOGW("journal open fail (%s)", path.c_str());
        return false;
    }
    
    mJournalIO->setReadMod();
    shared_ptr<Journal> journal;
    vector<shared_ptr<Journal>> listJournal;
    while ((journal=mJournalIO->readJournal()) != nullptr)
    {
        listJournal.push_back(journal);
    }
    JLOGD("Read %ld journals from %s", listJournal.size(), path.c_str());
    /** +1 for pushback a journal while writeMode
     */
    mJournalVector.reserve(listJournal.size()+1);
    mJournalVector.resize(listJournal.size());
    size_t i=0;
    for (auto &it:listJournal)
    {
        mJournalVector[i] = it;
        i++;
    }
    
    return true;
}

void SfJournalBook::close()
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    mJournalIO->close();
    mJournalVector.clear();
}

static bool fastSortDataCmp(const fastSortData &d1,const fastSortData &d2)
{
    return d1.stamp>d2.stamp;
}

void SfJournalBook::order()
{
    //todo : more flexable sort
    AutoLock aLock = AutoLock(mJournalVectorLock);
    vector<fastSortData> fsdVector = vector<fastSortData>(mJournalVector.size());
    size_t pos = 0;
    for (auto &it:mJournalVector)
    {
        fsdVector[pos].order = pos;
        fsdVector[pos].stamp = getStampFormConfig(it->getConfig());
        pos++;
    }
    sort(fsdVector.begin(), fsdVector.end(), fastSortDataCmp);
    vector<shared_ptr<Journal>> jTmpVector = vector<shared_ptr<Journal>>(mJournalVector);
    vector<shared_ptr<Journal>>::iterator jListIt = mJournalVector.begin();
    for (pos=0; pos<fsdVector.size(); pos++)
    {
        vector<shared_ptr<Journal>>::iterator jTmpIt = jTmpVector.begin()+fsdVector[pos].order;
        *jListIt = *jTmpIt;

        jListIt++;
    }
}

bool SfJournalBook::save()
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    if (!mJournalIO->setWriteMode())
        return false;

    for (auto &it:mJournalVector)
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
    AutoLock aLock = AutoLock(mJournalVectorLock);
    return mJournalVector.size();
}

shared_ptr<Journal> SfJournalBook::at(size_t pos)
{
    if (pos<0 || pos>= mJournalVector.size())
        return nullptr;
    return mJournalVector[pos];
}

shared_ptr<Journal> SfJournalBook::operator [](size_t pos)
{
    if (pos<0 || pos>= mJournalVector.size())
        return nullptr;
    return mJournalVector[pos];
}

bool SfJournalBook::insert(size_t pos, shared_ptr<Journal> journal)
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    if (pos<0 || pos> mJournalVector.size())
        return false;
    
    mJournalVector.insert(mJournalVector.begin()+pos, journal);
    return true;
}

void SfJournalBook::push_front(shared_ptr<Journal> journal)
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    mJournalVector.insert(mJournalVector.begin(), journal);
}

void SfJournalBook::push_back(shared_ptr<Journal> journal)
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    mJournalVector.push_back(journal);
}

void SfJournalBook::erase(size_t pos)
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    if (pos<0 || pos>=mJournalVector.size())
    {
        return;
    }
    
    vector<shared_ptr<Journal>>::iterator it = mJournalVector.begin()+pos;
    mJournalVector.erase(it);
}

bool SfJournalBook::swap(size_t pos1, size_t pos2)
{
    AutoLock aLock = AutoLock(mJournalVectorLock);
    if (pos1<0 || pos1>=mJournalVector.size()||
        pos2<0 || pos2>=mJournalVector.size())
        return false;
    
    vector<shared_ptr<Journal>>::iterator it1 = mJournalVector.begin()+pos1;
    vector<shared_ptr<Journal>>::iterator it2 = mJournalVector.begin()+pos2;
    

    iter_swap(it1, it2);
    return true;
}
