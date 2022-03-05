#ifndef JOURNAL_IO_API_H
#define JOURNAL_IO_API_H

#include <stddef.h>
#include "journal_struct.h"

#define FORMATE_TABLE_COLUM (8)

typedef void *(*allocate_instance_fnc)(void *handle);
typedef void (*release_instance_fnc)(void *);

typedef uint32_t (*apiSupport_fnc)(void *handle);
typedef char* (*formateSupport_fnc)(void *handle, size_t *line_num);
typedef bool (*isSupportAes256_fnc)(void *handle);
typedef void (*setKey_fnc)(void *handle, uint8_t key[32]);
typedef bool (*clearKey_fnc)(void *handle);
typedef void (*openIO_fnc)(void *handle, const char* path);
typedef void (*closeIO_fuc)(void *handle);
typedef bool (*readMode_fnc)(void *handle);
typedef bool (*writeMode_fnc)(void *handle);
typedef bool (*readJournal_fnc)(void *handle, struct journal_s*);
typedef bool (*writeJournal_fnc)(void *handle, struct journal_s*);

#endif