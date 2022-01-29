#ifndef TAG_CONFIG_NODE_H
#define TAG_CONFIG_NODE_H

#include "ConfigNodeBase.h"

using namespace ec;

class TagConfigNode : public ConfigNodeBase
{
public:
    TagConfigNode() = default;
    ~TagConfigNode() = default;

    uint32_t apiVersion() {return 1;};

    string getKey() {return string("tag");};
    string getDefaultValue() {return string("");};
    void preprocess(shared_ptr<Journal> journal) {return;};
    void postprocess(shared_ptr<Journal> journal) {return;};

    int32_t innerConfigType() {return TAG_CONFIG;};
};

#endif