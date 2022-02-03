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

    void setInitStr(string initStr);
    void clearInitStr();
    bool getTextFromEditor(string &strGot);
};
