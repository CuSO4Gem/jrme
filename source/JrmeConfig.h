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
#ifndef JRME_CONFIG_H
#define JRME_CONFIG_H

#include <list>
#include <string>

#include "iniparser.hpp"

using std::list;
using std::string;

/**
 * @brief
 * Jrme配置相关
 *
 * Jrme config
 *
 */
class JrmeConfig
{
public:
    /**
     * @brief
     * 初始化jrme的配置
     * init jrme config
     *
     * @return true install success or not need to install
     * @return false install fail
     */
    static bool installIfNeed();

    /**
     * @brief
     * 获取配置的根目录
     * Get the Config root dirstory
     *
     * @return string
     */
    static string getConfigRootDir();

    /**
     * @brief
     * 获取记录日记本的文件
     * get path of journak book list config file.
     *
     * @return string
     */
    static string getJournalBooksCfgPath();

    /**
     * @brief
     * 获取插件目录
     * Get the dir for save plugin
     *
     * @return string
     */
    static string getPluginDir();

    /**
     * @brief
     * 获取journalIO插件列表
     * Get config of the JournalIO Plugin Names
     *
     * @return list<string> 插件名称（不包括路径）。plugin name without path
     */
    static list<string> getJournalIOPluginNames();

    /**
     * @brief
     * 获取需要载入的JAttribute插件列表
     * Get the JAttribute Plugin Names
     *
     * @return list<string> 插件名称（不包括路径）。plugin name without path
     */
    static list<string> getConfigNodePluginNames();

    /**
     * @brief
     * 获取编辑器名
     * Get the Editor Name
     *
     * @return string
     */
    static string getEditorName();

    /**
     * @brief
     * 获取默认的日记本保存路径
     * Get the Default Journal Book Path
     *
     * @return string
     */
    static string getDefaultJournalBookPath();

    /**
     * @brief
     * 设置默认的日记本保存路径
     * Set the Deafult Journal Book Path
     *
     * @param[in] path 日记本文件路径。the journal book path
     */
    static void setDeafultJournalBookPath(string path);

    /**
     * @brief
     * 读取日记保存列表
     * Read journal book list
     *
     * @return list<string> 日记本文件路径。the journal book path
     */
    static list<string> readJournalBooksCfg();

    /**
     * @brief
     * 写入日记保存列表
     * Write journal book list
     *
     * @param[in] books 日记本文件路径列表。the journal book path list
     * @return true write success
     * @return false write fail
     */
    static bool writeJournalBooksCfg(list<string> allPath);

private:
    static bool   pathNormalize(string &rawPath, string &normalizedPath);
    static string getConfigFilePath();
    static string getInitJournalBookPath();
};

#endif