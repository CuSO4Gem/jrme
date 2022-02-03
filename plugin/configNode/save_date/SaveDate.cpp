#include <malloc.h>
#include <string>
#include <sstream>

#include "date.h"
#include "NodeUtil.h"
#include "SaveDate.h"

using namespace std;
using namespace ec;

#ifdef __cplusplus
extern "C" {
#endif

struct allocate_ret allocate_instance(void *handle)
{
    struct allocate_ret ret;
    ret.allocated = true;
    ret.pre_title_need_more = -1;
    ret.post_title_need_more = -1;
    ret.pre_config_need_more = -1;
    ret.post_config_need_more = 50;
    ret.pre_content_need_more = -1;
    ret.post_content_need_more = -1;
    return ret;
}

bool release_instance(void *handle)
{
    return true;
}

void preprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal)
{
    return;
}

void postprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal)
{    
    if (refJournal->config==NULL)
        return;
    
    if (strstr(refJournal->config, "save date")==NULL)
        return;
    
    if (retJournal->config == NULL || retJournal->config_size<=0);

    string config = string(refJournal->config);
    setValueToConfig(config, "save date", Time().toDate().toString());

    if (retJournal->config_size<config.length())
        return;
    
    memcpy(retJournal->config, config.c_str(), config.length());
    retJournal->config[config.length()] = '\0';
}

#ifdef __cplusplus
}
#endif