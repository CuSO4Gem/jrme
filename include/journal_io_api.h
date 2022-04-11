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
#ifndef JOURNAL_IO_API_H
#define JOURNAL_IO_API_H

#include <stddef.h>
#include "journal_struct.h"

/**
 * @brief 
 * 所支持格式的字符串长度。格式列表应该按照n*FORMATE_TABLE_COLUM保存
 * 
 * The string length of supported formats.
 * Table should be saved as n*FORMATE TABLE COLUM
 */
#define FORMATE_TABLE_COLUM (8)

/**
 * @brief 
 * JournalIO插件需要实现的API
 * JournalIO plugin API
 */
typedef void *(*allocate_instance_fnc)();
typedef void (*release_instance_fnc)(void *handle);

typedef uint32_t (*apiSupport_fnc)(void *handle);
typedef const char* (*formateSupport_fnc)(void *handle, size_t *line_num);
typedef bool (*isSupportAes256_fnc)(void *handle);
typedef void (*setKey_fnc)(void *handle, uint8_t key[32]);
typedef bool (*clearKey_fnc)(void *handle);
typedef bool (*openIO_fnc)(void *handle, const char* path);
typedef void (*closeIO_fnc)(void *handle);
typedef bool (*readMode_fnc)(void *handle);
typedef bool (*writeMode_fnc)(void *handle);
typedef bool (*readJournal_fnc)(void *handle, struct journal_s*);
typedef bool (*writeJournal_fnc)(void *handle, struct journal_s*);

#endif