#ifndef UTILS_H
#define UTILS_H

#include "date.h"
#include "Journal.h"

#include <string>

using namespace::std;
using namespace::ec;

/**
 * @brief remove multiple space from string
 */
void removeMultipleSpaces(string &str);

Date getJournalFormConfig(string &config);

#endif