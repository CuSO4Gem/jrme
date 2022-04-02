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
#ifndef CONFIG_NODE_MASTER_H
#define CONFIG_NODE_MASTER_H

#include <list>
#include <memory>

#include "ConfigNodeBase.h"

class ConfigNodeMaster
{
private:
    list<shared_ptr<ConfigNodeBase>> mNodeList;

public:
    ConfigNodeMaster();
    ~ConfigNodeMaster() = default;

    size_t nodeSize();

    /**
     * @brief add plug in node to ConfigNodeMaster
     * 
     * @param name plugin node
     * @return true load plugin success
     * @return false load plugin faild
     */
    bool addPluginNode(string name);
    bool addPluginNode(list<string> nameList) {return true;};

    string genConfig();
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);

    void setDate(time_t stamp);
};

#endif