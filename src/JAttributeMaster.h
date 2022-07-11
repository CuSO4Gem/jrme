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

#ifndef JATTRIBUTE_MASTER_H
#define JATTRIBUTE_MASTER_H

#include <list>
#include <memory>

#include "JAttributeBase.h"

/**
 * @brief 
 * 加载所有的JAttribute，包括插件
 * 管理搜哟逇JAttribute
 * 生成日记的attributePart部分
 * 依次调用attributePart node的preprocess()和postprecess()
 * 
 * Load all JAttribute include pulgin.
 * JAttributeMaster manage all JAttribute.
 * Generate JAttribute of a journal.
 * Call preprocess() and postprocess() one by one.
 * 
*/
class JAttributeMaster
{
private:
    list<shared_ptr<JAttributeBase>> mNodeList;

public:
    JAttributeMaster();
    ~JAttributeMaster() = default;

    size_t attributeSize();

    /**
     * @brief 载入一个JAttribute插件
     * add plug in node to JAttributeMaster
     * 
     * @param name plugin node
     * @return true load plugin success
     * @return false load plugin faild
     */
    bool addPluginNode(string name);
    bool addPluginNode(list<string> nameList) {return true;};

    /**
     * @brief 根据载入的JAttribute生成日记中的config部分
     * Generate attributePart in journal by loaded attributePart node
     * 
     * @return string 
     */
    string genJAttributePart();
    /**
     * @brief 依次运行每个载入的JAttribute的preprocess()或者postprocess()
     * Run preprocess() or postprocess() one by one
     */
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);

    /**
     * @brief 为 date JAttribute设置时间
     * set date for date JAttribute.
     */
    void setDate(time_t stamp);
};

#endif