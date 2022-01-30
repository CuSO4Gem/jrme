#ifndef JRME_CONFIG_H
#define JRME_CONFIG_H

#include <string>

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
 * @brief 
 * 
 */

/**
 * @brief install to init jrme, if need
 * 
 * @return true install success or not need to install
 * @return false install fail
 */
bool installIfNeed();

#endif