#ifndef SAVE_DATE_H
#define SAVE_DATE_H

#include "config_node_api.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#pragma GCC visibility push(default)

extern const uint32_t api_version = 1;
extern const char key[] = "save date";
extern const char default_value[] = "auto_fill";

struct allocate_ret allocate_instance(void *handle);
bool release_instance(void *handle);
void preprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal);
void postprocess(void *handle, const struct journal_cs *refJournal, struct journal_s *retJournal);

#pragma GCC visibility pop
#ifdef __cplusplus
}
#endif

#endif