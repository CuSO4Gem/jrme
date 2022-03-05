#ifndef CONFIG_NODE_API_H
#define CONFIG_NODE_API_H

#include <stdint.h>
#include "journal_struct.h"

typedef void *(*allocate_instance_fnc)();
typedef void (*release_instance_fnc)(void *);
/**
 * @brief handle, input journal, journal after preprocess
 */
typedef void (*preprocess_fnc)(void *, const struct journal_s *, struct journal_s *);
/**
 * @brief handle, input journal, journal after postprocess
 */
typedef void (*postprocess_fnc)(void *, const struct journal_s *, struct journal_s *);

#endif