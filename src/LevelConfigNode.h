#ifndef LEVEL_CONFIG_NODE_H
#define LEVEL_CONFIG_NODE_H

#include "ConfigNodeBase.h"

using namespace ec;

class LevelConfigNode : public ConfigNodeBase
{
public:
    LevelConfigNode() = default;
    ~LevelConfigNode() = default;

    uint32_t apiVersion() {return 1;};

    string getKey() {return string("level");};
    string getDefaultValue() {return string("");};
    void preprocess(shared_ptr<Journal> journal) {return;};
    void postprocess(shared_ptr<Journal> journal) {return;};

    int32_t innerConfigType() {return LEVEL_CONFIG;};
};

#endif