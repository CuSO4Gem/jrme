#include <string>
using namespace std;

class TxtEditor
{
private:
    string mInitStr;

public:
    TxtEditor() = default;
    TxtEditor(string initStr);
    ~TxtEditor();

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
     * @param strGot [out]: 返回用户的输入。如果用户不希望保存，则没有内容。 
     * @return true 用户希望保存
     * @return false 用户不希望保存
     */
    bool getTextFromEditor(string &strGot);
};
