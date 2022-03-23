#ifndef JRME_CONFIG_H
#define JRME_CONFIG_H

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
    static string journalBooksPathCfg();

public:
    /**
     * @brief Get the Config Root dirstory
     * 
     * @return string 
     */
    static string getConfigRootDir();

    /**
     * @brief Get the dir for save plugin
     * 
     * @return string 
     */
    static string getPluginDir();

    static string getEditorName();

    static vector<string> getConfigNodePluginNames();

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
    static vector<string> getJournalIOPluginNames();

    /**
     * @brief Get the Journal Books path from config.
     * 
     * @return vector<string> 
     */
    static vector<string> getJournalBooksPath();

    /**
     * @brief Save journal books path to config
     * 
     */
    static void saveJournalBooksPath(vector<string> allPath);
};

#endif