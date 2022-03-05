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

void *allocate_instance();
void release_instance(void *handle);
/**
 * @brief handle, input journal, journal after preprocess
 */
void preprocess(void *handle, const struct journal_s *refJournal, struct journal_s *retJournal);
/**
 * @brief handle, input journal, journal after postprocess
 */
void postprocess(void *handle, const struct journal_s *refJournal, struct journal_s *retJournal);

#pragma GCC visibility pop
#ifdef __cplusplus
}
#endif

#endif