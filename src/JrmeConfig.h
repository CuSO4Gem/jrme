#ifndef JRME_CONFIG_H
#define JRME_CONFIG_H

#include <string>

#include "iniparser.hpp"

using namespace std;

string getConfigRootDir();

string getConfigPath();

bool installIfNeed();

#endif