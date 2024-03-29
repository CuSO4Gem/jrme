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
#ifndef WRITE_MODE_H
#define WRITE_MODE_H

#include <string>

using std::string;

/**
 * @brief
 * 写日记
 * Wirte journal
 */
int journlWriteMode(string bookPath, string timeDescription, string title, string content);

#endif