#include "ConfigNodeMaster.h"
#include "DateConfigNode.h"
#include "LevelConfigNode.h"
#include "TagConfigNode.h"
/*
class PluginNode : public ConfigNodeBase
{
private:
public:
    PluginNode();
    ~PluginNode();

    bool loadPlugin(string path);
};*/

ConfigNodeMaster::ConfigNodeMaster()
{
    mNodeList.push_back(make_shared<DateConfigNode>());
    mNodeList.push_back(make_shared<TagConfigNode>());
    mNodeList.push_back(make_shared<LevelConfigNode>());

    //todo : load plugin

    for (auto &it:mNodeList)
    {
        mConfig.append(it->getKey() + "=" + it->getDefaultValue() + "\n");
    }
}

size_t ConfigNodeMaster::nodeSize()
{
    return mNodeList.size();
}

string ConfigNodeMaster::genConfig()
{
    return mConfig;
}

void ConfigNodeMaster::preprocess(shared_ptr<Journal> journal)
{
    for (auto &it:mNodeList)
    {
        it->preprocess(journal);
    }
    return;
}

void ConfigNodeMaster::postprocess(shared_ptr<Journal> journal)
{
    for (auto &it:mNodeList)
    {
        it->postprocess(journal);
    }
    return;
}

void ConfigNodeMaster::setDate(time_t stamp)
{
    for (auto &it:mNodeList)
    {
        it->setDate(stamp);
    }
    return;
}
