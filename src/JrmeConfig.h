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

class JrmeConfig
{
private:
    static bool pathNormalize(string &rawPath, string &normalizedPath);
    static string getConfigFilePath();
    static string getInitJournalBookPath();

public:
    /**
     * @brief Get the Config Root dirstory
     * 
     * @return string 
     */
    static string getConfigRootDir();

    /**
     * @brief get path of journak book list config file.
     * 
     */
    static string getJournalBooksCfgPath();

    /**
     * @brief Get the dir for save plugin
     * 
     * @return string 
     */
    static string getPluginDir();

    static string getEditorName();

    static list<string> getConfigNodePluginNames();

    static string getDefaultJournalBookPath();
    
    static void setDeafultJournalBookPath(string path);

    /**
     * @brief install to init jrme, if need
     * 
     * @return true install success or not need to install
     * @return false install fail
     */
    static bool installIfNeed();

    /**
     * @brief Get config of the JournalIO Plugin Name 
     * 
     * @return vector<string> 
     */
    static list<string> getJournalIOPluginNames();

    /**
     * @brief Get the Journal Books path from config.
     * 
     * @return vector<string> 
     */
    static list<string> readJournalBooksCfg();

    /**
     * @brief Save journal books path to config
     * 
     */
    static bool writeJournalBooksCfg(list<string> allPath);
};

#endif