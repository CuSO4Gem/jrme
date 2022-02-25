#ifndef TXT_EDITOR_H
#define TXT_EDITOR_H
#include <string>
#include <memory>

#include "Journal.h"

using namespace std;

class TxtEditor
{
private:
    string mInitStr;

public:
    TxtEditor() = default;
    TxtEditor(string initStr);
    ~TxtEditor() = default;

    /**
     * @brief 设置editor初始的字符串
     * 
     * @param initStr 
     */
    void setInitStr(string initStr);

    /**
     * @brief 清除editor初始的字符串
     * 
     */
    void clearInitStr();

    /**
     * @brief 让用户从editor输入日记
     * 
     * @return nullptr 用户不希望保存
     */
    shared_ptr<Journal> getJournalFromEditor();
};

#endif