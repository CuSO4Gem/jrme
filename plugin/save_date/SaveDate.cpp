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
#include <malloc.h>
#include <string>
#include <sstream>

#include "date.h"
#include "SaveDateUtil.h"
#include "SaveDate.h"

using namespace std;
using namespace ec;

struct save_date_s
{
    uint32_t node;
};


#ifdef __cplusplus
extern "C" {
#endif

void *allocate_instance()
{
    struct save_date_s *save_data = (struct save_date_s *)malloc(sizeof(struct save_date_s));
    return save_data;
}

void release_instance(void *handle)
{
    struct save_date_s *save_data = (struct save_date_s *)handle;
    if (!save_data)
        return;

    free(save_data);
}

void releaseJournalStruct(struct journal_s journal)
{
    if (journal.title)
        free(journal.title);
    if (journal.attributePart)
        free(journal.attributePart);
    if (journal.content)
        free(journal.content);
}

void preprocess(void *handle, const struct journal_s *refJournal, struct journal_s *retJournal)
{
    return;
}

void postprocess(void *handle, const struct journal_s *refJournal, struct journal_s *retJournal)
{
    struct save_date_s *save_data = (struct save_date_s *)handle;

    string attributePart = string(refJournal->attributePart);
    setValueToJAttributePart(attributePart, "save date", Time().toDate().toString());
    retJournal->attributePart = (char *)malloc(attributePart.length()+1);
    memcpy(retJournal->attributePart, attributePart.c_str(), attributePart.length());
    (retJournal->attributePart)[attributePart.length()] = '\0';

/**
 * @note 
 * retJournal不需要插件主动调用releaseJournalStruct()释放，
 * jrme会调用插件的releaseJournalStruct()。
 * 
 * retJournal does not require the plugin to actively call releaseJournalStruct() to release,
 * jrme will call releaseJournalStruct() in plugin.
 */
}

#ifdef __cplusplus
}
#endif