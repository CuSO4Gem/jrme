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

void SaveDate::preprocess(const struct journal_cs *refJournal, struct journal_s *retJournal)
{
    *retJournal = mPreRetJournal;
    return;
}

void SaveDate::postprocess(const struct journal_cs *refJournal, struct journal_s *retJournal)
{/*
    if (refJournal->config==NULL)
        return;
    
    if (strstr(refJournal->config, "save date")==NULL)
        return;
    
    string config = string(refJournal->config);
    setValueToConfig(config, "save date", Time().toDate().toString());
    printf("debug setValueToConfig end\n");
    if (!mPostRetJournal.config)
    {
        printf("debug malloc\n");
        mPostRetJournal.config = (char*)malloc(config.length()+1);
    }
    else
    {
        printf("debug realloc\n");
        mPostRetJournal.config = (char*)realloc(mPostRetJournal.config, config.length()+1);
    }
    
    printf("debug befor memcpy\n");
    memcpy(mPostRetJournal.config, config.c_str(), config.length());
    mPostRetJournal.config[config.length()] = '\0';
    retJournal->config = mPostRetJournal.config;
    */
    // retJournal->config = mPostRetJournal.config;
    char *tmp = (char *)malloc(10);
    // mPostRetJournal.config = (char *)malloc(10);
    // printf("mPostRetJournal.config 1=%d", mPostRetJournal.config);
    // mPostRetJournal.config = NULL;
    printf("mPostRetJournal.config 2=%d\n", mPostRetJournal.config);
    // retJournal->config = tmp;
    return;
}

#ifdef __cplusplus
extern "C" {
#endif

bool allocate_instance(void *handle)
{
    handle = new SaveDate();
    return true;
}

bool release_instance(void *handle)
{
    if (handle==NULL)
        return false;

    delete (SaveDate*)handle;
    handle = NULL;
    return true;
}

void preprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal)
{
    return;
}

void postprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal)
{
    SaveDate *inst = (SaveDate*)handle;
    inst->postprocess(refJournal, retJournal);
}

#ifdef __cplusplus
}
#endif