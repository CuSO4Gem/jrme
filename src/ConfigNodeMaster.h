#ifndef CONFIG_NODE_MASTER_H
#define CONFIG_NODE_MASTER_H

#include <list>
#include <memory>

#include "ConfigNodeBase.h"

class ConfigNodeMaster
{
private:
    list<shared_ptr<ConfigNodeBase>> mNodeList;

public:
    ConfigNodeMaster();
    ~ConfigNodeMaster() = default;

    size_t nodeSize();

    /**
     * @brief add plug in node to ConfigNodeMaster
     * 
     * @param name plugin node
     * @return true load plugin success
     * @return false load plugin faild
     */
    bool addPluginNode(string name);
    bool addPluginNode(list<string> nameList) {return true;};

    string genConfig();
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);

    void setDate(time_t stamp);
};

#endif