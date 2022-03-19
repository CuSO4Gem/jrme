#ifndef JRME_CONFIG_H
#define JRME_CONFIG_H

#include <string>
#include <vector>

#include "iniparser.hpp"

using namespace std;

/**
 * @brief Get the Config Root dirstory
 * 
 * @return string 
 */
string getConfigRootDir();

/**
 * @brief Get the Config Path of jrme
 * 
 * @return string 
 */
string getConfigFilePath();

/**
 * @brief Get the dir for save plugin
 * 
 * @return string 
 */
string getPluginDir();

/**
 * @brief install to init jrme, if need
 * 
 * @return true install success or not need to install
 * @return false install fail
 */
bool installIfNeed();

/**
 * @brief Get config of the JournalIO Plugin Name 
 * 
 * @return vector<string> 
 */
vector<string> getJournalIOPluginNames();

#endif