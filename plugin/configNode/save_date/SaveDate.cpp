#include <malloc.h>
#include <string>
#include <sstream>

#include "date.h"
#include "NodeUtil.h"
#include "SaveDate.h"

using namespace std;
using namespace ec;

struct save_date_s
{
    char *config_malloc;
};


#ifdef __cplusplus
extern "C" {
#endif

void *allocate_instance()
{
    struct save_date_s *save_data = (struct save_date_s *)malloc(sizeof(struct save_date_s));
    save_data->config_malloc = NULL;
    return save_data;
}

void release_instance(void *handle)
{
    struct save_date_s *save_data = (struct save_date_s *)handle;
    if (!save_data)
        return;

    if (save_data->config_malloc)
    {
        free(save_data->config_malloc);
    }
    free(save_data);
}

void preprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal)
{
    return;
}

void postprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal)
{
    struct save_date_s *save_data = (struct save_date_s *)handle;

    string config = string(refJournal->config);
    setValueToConfig(config, "save date", Time().toDate().toString());
    retJournal->config = (char *)malloc(config.length()+1);
    memcpy(retJournal->config, config.c_str(), config.length());
    (retJournal->config)[config.length()] = '\0';
    if (save_data->config_malloc)
        free(save_data->config_malloc);
    
    save_data->config_malloc = retJournal->config;
}

#ifdef __cplusplus
}
#endif