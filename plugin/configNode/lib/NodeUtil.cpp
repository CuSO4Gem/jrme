#include "NodeUtil.h"
#include "sstream"

void tabToSpace(string &str)
{
    size_t pos;
    while (pos<str.length())
    {
        if (str[pos] == '\t')
        pos++;
    }
}

bool configGetline(istringstream &configStream, string &key, string &value)
{
    string lineBuffer;
    key.clear();
    value.clear();
    if(!getline(configStream, lineBuffer))
        return false;
    
    if (lineBuffer.length() == 0)
        return true;
    
    tabToSpace(lineBuffer);
    size_t pos = 0;
    while (pos<lineBuffer.length())
    {
        if (lineBuffer[pos]=='=')
        {
            key = lineBuffer.substr(0, pos);
            break;
        }
        pos++;
    }

    lineBuffer.erase(0, min(pos+1, lineBuffer.length()));
    
    /*remove space front and back*/
    while (key.length()>0 && key[0]==' ')
    {
        key.erase(0);
    }
    while (key.length()>0 && key[key.length()-1]==' ')
    {
        key.erase(key.length()-1);
    }

    if (lineBuffer.length()==0)
        return true;
    
    value = lineBuffer;
    /*remove space front and back*/
    while (value.length()>0 && value[0]==' ')
    {
        value.erase(0);
    }
    while (value.length()>0 && value[value.length()-1]==' ')
    {
        value.erase(value.length()-1);
    }
    return true;
}

bool setValueToConfig(string &config, const string &key,const string value)
{
    string keyBuffer, valueBuffer, lineBuffer;
    istringstream configStream = istringstream(config);
    
    bool finded = false;
    size_t keyBeginPos = configStream.tellg();
    while(configGetline(configStream, keyBuffer, valueBuffer))
    {
        if (keyBuffer==key)
        {
            finded = true;
            break;
        }
        keyBeginPos = configStream.tellg();
    }
    if (!finded)
        return false;
    
    configStream.seekg(keyBeginPos);
    getline(configStream, lineBuffer);
    size_t len = lineBuffer.length();
    config.erase(keyBeginPos, len);
    string insertString = key+"="+value;
    config.insert(keyBeginPos, insertString);

    return true;
}