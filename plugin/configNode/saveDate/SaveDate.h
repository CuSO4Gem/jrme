#ifndef SAVE_DATE_H
#define SAVE_DATE_H

#include "plugin_common.h"
#include "config_node_api.h"

#include <string.h>

class SaveDate
{
private:
    
public:
    SaveDate();
    ~SaveDate();
    struct journal_s mPreRetJournal;
    struct journal_s mPostRetJournal;

    void preprocess(const struct journalIn_s *refJournal, struct journal_s *retJournal);
    void postprocess(const struct journalIn_s *refJournal, struct journal_s *retJournal);
};

#ifdef __cplusplus
extern "C" {
#endif

JRME_PLUGIN_API const uint32_t api_version = 1;
JRME_PLUGIN_API const char s_key[] = "save date";
JRME_PLUGIN_API const char s_default_value[] = "auto_fill";

JRME_PLUGIN_API bool allocate_instance(void *handle);
JRME_PLUGIN_API bool release_instance(void *handle);
JRME_PLUGIN_API void preprocess(void *handle, const struct journalIn_s *refJournal, struct journal_s *retJournal);
JRME_PLUGIN_API void postprocess(void *handle, const struct journalIn_s *refJournal, struct journal_s *retJournal);

#ifdef __cplusplus
}
#endif

#endif