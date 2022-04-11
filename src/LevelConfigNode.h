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
#ifndef LEVEL_CONFIG_NODE_H
#define LEVEL_CONFIG_NODE_H

#include "ConfigNodeBase.h"

using namespace ec;

/**
 * @brief 
 * "level"关键字的config node
 * "level" config node
 */
class LevelConfigNode : public ConfigNodeBase
{
public:
    LevelConfigNode() = default;
    ~LevelConfigNode() = default;

    uint32_t apiVersion() {return 1;};

    string getKey() {return string("level");};
    string getDefaultValue() {return string("");};
    void preprocess(shared_ptr<Journal> journal) {return;};
    void postprocess(shared_ptr<Journal> journal) {return;};

    int32_t innerConfigType() {return LEVEL_CONFIG;};
};

#endif