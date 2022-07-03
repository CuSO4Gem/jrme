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
#ifndef JOURNAL_STRUCT_H
#define JOURNAL_STRUCT_H

#include <malloc.h>

/**
 * @brief 
 * 用于插件与jrme传递jrounal
 * Deliver bewteen jrme and plugin
 */
struct journal_s
{
    char *title;
    char *config;
    char *content;
};

typedef void (*releaseJournal_fnc)(struct journal_s);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 
 * 插件和jrme都需要实现自己的releaseJournalStruct()函数。
 * 
 * plugin and jrme need to impelement releaseJournalStruct() function.
 */
void releaseJournalStruct(struct journal_s journal);
#ifdef __cplusplus
}
#endif

#endif