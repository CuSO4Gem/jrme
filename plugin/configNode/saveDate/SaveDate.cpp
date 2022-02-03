#include <malloc.h>
#include <string>
#include <sstream>

#include "date.h"
#include "NodeUtil.h"
#include "SaveDate.h"


using namespace std;
using namespace ec;

SaveDate::SaveDate()
{    
    mPreRetJournal.title = NULL;
    mPreRetJournal.config = NULL;
    mPreRetJournal.content = NULL;

    mPostRetJournal.title = NULL;
    mPostRetJournal.config = NULL;
    mPostRetJournal.content = NULL;
    return;
}

SaveDate::~SaveDate()
{
    if (mPreRetJournal.title)
        free(mPreRetJournal.title);
    if (mPreRetJournal.config)
        free(mPreRetJournal.config);
    if (mPreRetJournal.content)
        free(mPreRetJournal.content);
    
    if (mPostRetJournal.title)
        free(mPostRetJournal.title);
    if (mPostRetJournal.config)
        free(mPostRetJournal.config);
    if (mPostRetJournal.content)
        free(mPostRetJournal.content);
        
    return;
}

void SaveDate::preprocess(const struct journalIn_s *refJournal, struct journal_s *retJournal)
{
    *retJournal = mPreRetJournal;
    return;
}

void SaveDate::postprocess(const struct journalIn_s *refJournal, struct journal_s *retJournal)
{
    if (refJournal->config==NULL)
        return;
    
    if (strstr(refJournal->config, "save date")==NULL)
        return;
    
    string config = string(refJournal->config);
    setValueToConfig(config, "save date", Time().toDate().toString());
    mPostRetJournal.config = (char*)malloc(config.length()+1);
    memcpy(mPostRetJournal.config, config.c_str(), config.length());
    mPostRetJournal.config[config.length()] = '\0';

    *retJournal = mPostRetJournal;
    return;
}

bool allocate_instance(void *handle)
{
    handle = new SaveDate();
    return true;
}

bool release_instance(void *handle)
{
    if (handle==NULL)
        return false;

    // delete const_cast<SaveDate*>(handle);
    return true;
}

void preprocess(void *handle, const struct journalIn_s *refJournal, struct journal_s *retJournal)
{
    return;
}

void postprocess(void *handle, const struct journalIn_s *refJournal, struct journal_s *retJournal)
{
    SaveDate *inst = (SaveDate*)handle;
    inst->postprocess(refJournal, retJournal);
}