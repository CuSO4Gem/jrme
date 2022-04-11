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
#ifndef CONFIG_NODE_API_H
#define CONFIG_NODE_API_H

#include <stdint.h>
#include "journal_struct.h"

/**
 * @brief 
 * config node插件的api
 * 插件还需要额外暴露以下内容
 * 
 * api of config node plugin
 * plugin also need the following elements.
 * 
 * uint32_t api_version
 * char key[]
 * char default_value[]
 */
typedef void *(*allocate_instance_fnc)();
typedef void (*release_instance_fnc)(void *);
typedef void (*preprocess_fnc)(void *, const struct journal_s *, struct journal_s *);
typedef void (*postprocess_fnc)(void *, const struct journal_s *, struct journal_s *);

#endif