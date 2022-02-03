#ifndef CONFIG_NODE_BASE_H
#define CONFIG_NODE_BASE_H

#include <memory>
#include <string>
#include <Journal.h>
using namespace std;

enum innerConfigNode {
    DATE_CONFIG = 1,
    TAG_CONFIG,
    LEVEL_CONFIG,
};

class ConfigNodeBase
{
public:
    ~ConfigNodeBase() = default;

    virtual uint32_t apiVersion() = 0;

    virtual string getKey() = 0;
    virtual string getDefaultValue() = 0;
    virtual void preprocess(shared_ptr<Journal> journal) = 0;
    virtual void postprocess(shared_ptr<Journal> journal) = 0;

    virtual int32_t innerConfigType()  {return 0;};
    virtual void setDate(time_t stamp) {};
};
#endif