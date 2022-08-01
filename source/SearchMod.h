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
#ifndef SEARCH_MOD_H
#define SEARCH_MOD_H

#include <string>

#include "cmdline.h"

using namespace std;

/**
 * @brief 
 * 日记搜索功能
 * search journal
 */
int journalSearchMod(cmdline::parser &cmd, string bookPath);

#endif