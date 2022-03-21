#ifndef JRME_CONFIG_H
#define JRME_CONFIG_H

#include <string>
#include <vector>

#include "iniparser.hpp"

using namespace std;

class JrmeConfig
{
private:
    static string getDefaultJournalPath();
    static bool pathNormalize(string &rawPath, string &normalizedPath);

public:
    /**
     * @brief Get the Config Root dirstory
     * 
     * @return string 
     */
    static string getConfigRootDir();

    /**
     * @brief Get the Config Path of jrme
     * 
     * @return string 
     */
    static string getConfigFilePath();

    /**
     * @brief Get the dir for save plugin
     * 
     * @return string 
     */
    static string getPluginDir();

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
    static vector<string> getJournalIOPluginNames();

};

#endif