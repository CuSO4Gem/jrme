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
#include <vector>

#include "iniparser.hpp"

using namespace std;

/**
 * @brief 
 * Jrme配置相关
 * 
 * Jrme config
 * 
 */
class JrmeConfig
{
private:
    static bool pathNormalize(string &rawPath, string &normalizedPath);
    static string getConfigFilePath();
    static string getInitJournalBookPath();

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
     * @return string 
     */
    static string getConfigRootDir();

    /**
     * @brief 
     * 获取记录日记本的文件
     * get path of journak book list config file.
     */
    static string getJournalBooksCfgPath();

    /**
     * @brief 
     * 获取插件目录
     * Get the dir for save plugin
     */
    static string getPluginDir();

    /**
     * @brief 
     * 获取journalIO插件列表
     * Get config of the JournalIO Plugin Names 
     */
    static list<string> getJournalIOPluginNames();

    /**
     * @brief 
     * 获取需要载入的config node插件列表
     * Get the Config Node Plugin Names
     */
    static list<string> getConfigNodePluginNames();

    /**
     * @brief 
     * 获取编辑器名
     * Get the Editor Name
     */
    static string getEditorName();

    /**
     * @brief 
     * 获取默认的日记本保存路径
     * Get the Default Journal Book Path
     */
    static string getDefaultJournalBookPath();
    
    /**
     * @brief 
     * 设置默认的日记本保存路径
     * Set the Deafult Journal Book Path
     */
    static void setDeafultJournalBookPath(string path);


    /**
     * @brief 
     * 读取日记保存列表
     * Read journal book list
     */
    static list<string> readJournalBooksCfg();

    /**
     * @brief
     * 写入日记保存列表
     * Write journal book list
     */
    static bool writeJournalBooksCfg(list<string> allPath);
};

#endif