#ifndef CONFIG_NODE_MASTER_H
#define CONFIG_NODE_MASTER_H

#include <list>
#include <memory>

#include "ConfigNodeBase.h"

class ConfigNodeMaster
{
private:
    list<shared_ptr<ConfigNodeBase>> mNodeList;
    string mConfig;

public:
    ConfigNodeMaster();
    ~ConfigNodeMaster() = default;

    size_t nodeSize();
    bool loadPlugin(string name) {return true;};
    bool loadPlagin(list<string> nameList) {return true;};

    string genConfig();
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);

    void setDate(time_t stamp);
};

#endif