#include <algorithm>
#include "date.h"
#include <stdio.h>
#include <time.h>
#include "timeparser_plugin.h"
#include "parseRegex.h"

using namespace ec;
using namespace std;

#define ALL_FULL_MONTH_REGEX "(january|february|march|april|may|june|july|august|september|october|november|december)"
#define ALL_SHORT_MONTH_REGEX "(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)"

constexpr int32_t TWOCC(const char *s)
{
    return  (unsigned char)*(s)|
            (unsigned char)*(s+1)<<8;
}

inline bool isNumChar(const char ch)
{
    return '0'<=ch && ch<='9';
}

class Parser
{
public:
    Parser(const char *intStr);
    int getTime(ptm timeGot);

private:
    string mInStr;
    string mRemainStr;
    string mVmStr;
    Date mTargetTime;
    int32_t mParseFlag;
    
    void parseVM();
    void prepare();
    void parseAmPm();

    void removeMultipleSpaces(string &str);
};

Parser::Parser(const char *intStr)
{
    mInStr = string(intStr);
    transform(mInStr.begin(), mInStr.end(), mInStr.begin(), ::tolower);
    mRemainStr = "";
    mVmStr = string("");
}


int Parser::getTime(ptm timeGot)
{
    if (timeGot==NULL || mInStr.length()==0)
    {
        return TIME_PARSE_PARAMETER_ERROR;
    }
    prepare();
    parseAmPm();
    return 0;
}

void Parser::parseVM()
{
    /*
    am:
    pm
    yy:year
    mo:month
    md:day of month
    ww:week
    wd:day of month
    hh:hour
    hn:not speic hour, such as "night"
    mi:minute
    ss:second
    */
    return;
}

/*
    字符预处理
    in,on,at -> in
*/
void Parser::prepare()
{
    uint32_t spaceAccount=0;
    for (size_t i = 0; i < mInStr.length(); i++)
    {
        char ch;
        if (mInStr[i]==' ' || mInStr[i]=='\t')
            spaceAccount++;
        else
            spaceAccount=0;

        if(spaceAccount>1 || mInStr[i]=='|')
            continue;
        else if(mInStr[i]=='\t')
            ch = ' ';
        else
            ch = mInStr[i];

        mRemainStr.append(string(&ch));
    }
    
    while (mRemainStr.find("on ")!=string::npos)
    {
        mRemainStr.replace(mRemainStr.find("on "), 2, "in");
    }
    while (mRemainStr.find("at ")!=string::npos)
    {
        mRemainStr.replace(mRemainStr.find("at "), 2, "in");
    }
}

void Parser::parseAmPm()
{
    int pos=0;
    while ( (pos=mRemainStr.find("am", pos))!=string::npos )
    {
        //一个合格的am，前后应该只有空格或者数字
        if((pos==0 || isNumChar(mRemainStr[pos-1]) || mRemainStr[pos-1]==' ') &&
           (mRemainStr[pos+2]=='\0' || isNumChar(mRemainStr[pos+2]) || mRemainStr[pos+2]==' '))
        {
            mVmStr.append("|am");
            mRemainStr = mRemainStr.replace(pos, 2, " ");
        }
        else
            pos++;
    }

    pos=0;
    while ( (pos=mRemainStr.find("pm", pos))!=string::npos )
    {
        //一个合格的pm，前后应该只有空格或者数字
        if((pos==0 || isNumChar(mRemainStr[pos-1]) || mRemainStr[pos-1]==' ') &&
           (mRemainStr[pos+2]=='\0' || isNumChar(mRemainStr[pos+2]) || mRemainStr[pos+2]==' '))
        {
            mVmStr.append("|pm");
            // mRemainStr = mRemainStr.replace(pos, 2, " ");
            printf("pm:%s\r\n", mRemainStr.replace(pos, 2, " ").c_str());
        }
        else
            pos++;
    }
    removeMultipleSpaces(mRemainStr);
}

void Parser::removeMultipleSpaces(string &str){
    uint32_t i=0;
    uint32_t gotSpace=0;
    while (i<str.length())
    {
        if(str[i]==' ')
            gotSpace++;
        else
            gotSpace=0;
        
        if (gotSpace>1)
        {
            str.erase(i,1);
        }
        else
        {
            i++;
        }
    }
}

int32_t parseRegex(const char *str, uint32_t len ,ptm ptime)
{
    Time nowTime;
    Date date = nowTime.toDate();
    date.setMonth(2);
    
    ptime->tm_year = date.year()-1900;
    ptime->tm_mon = date.month();
    ptime->tm_mday = date.day();
    
    struct tm timeGoten;
    Parser timeParser(str);
    timeParser.getTime(&timeGoten);
    
    return 4;
}