#include <algorithm>
#include "date.h"
#include <regex>
#include <stdio.h>
#include <time.h>
#include "timeparser_plugin.h"
#include "parseRegex.h"

using namespace ec;
using namespace std;

#define ALL_SHORT_MONTH_REGEX "(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)"
char monthShortName[][4] = {
    "jan",
    "feb",
    "mar",
    "apr",
    "may",
    "jun",
    "jul",
    "aug",
    "sep",
    "oct",
    "nov",
    "dec"
};

char weekShortName[][4] = {
    "mon",
    "the",
    "wed",
    "thu",
    "fir",
    "sat",
    "sun"
};

constexpr int32_t TWOCC(const char *s)
{
    return  (unsigned char)*(s)|
            (unsigned char)*(s+1)<<8;
}

inline bool isNumChar(const char ch)
{
    return '0'<=ch && ch<='9';
}

inline bool isLetter(const char ch)
{
    return ('a'<=ch && ch<='z') || ('A'<=ch && ch<='Z');
}

inline bool isLetterOnly(const string &str)
{
    size_t i;
    for (i = 0; i < str.length(); i++)
    {
        if(!isLetter(str[i]))
            break;
    }
    if (i<str.length())
        return false;
    else
        return true;
}
/**
 * @brief 找到从pos开始的下一个单词开头（不计算开通的空格）
 * @return size_t 下一个单词的开头
 */
size_t nextWord(const string &str, size_t pos)
{
    if (pos>=str.length())
    {
        return string::npos;
    }
    /*skip space in front*/
    while (str[pos]==' ' && pos<str.length())
    {
        pos++;
    }
    if (pos>=str.length())
        return string::npos;
    
    /*skip the first word*/
    while (pos<str.length())
    {
        if (isNumChar(str[pos]) || isLetter(str[pos]))
            break;
        pos++;
    }
    if (pos>=str.length())
        return string::npos;
    
    /*skip space*/
    while (str[pos]==' ' && pos<str.length())
    {
        pos++;
    }
    if (pos>=str.length())
        return string::npos;
    
    if (pos>=str.length())
        return string::npos;
    else
        return pos;
}

/**
 * @brief 获取字符串的第一个单词
 * 
 * @param str 
 * @return string 第一个单词
 */
string getWord(const string &str)
{
    size_t pos = 0;
    /*skip space in front*/
    while (str[pos]==' ' && pos<str.length())
    {
        pos++;
    }
    if (pos>=str.length())
        return string("");
    size_t begin = pos;


    while (str[pos]!=' ' && pos<str.length())
    {
        pos++;
    }
    if (pos>=str.length())
        return string("");
    size_t end = pos;
    
    return str.substr(begin, end-begin);
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
    void parseAmPm(string &words, string &instructions);
    int32_t parseSampleUnit(const string words, string &instructions);
    void parseFormat(string &words, string &instructions);

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

    vector<string> instructionsList;
    string instructions;
    parseAmPm(mRemainStr, instructions);
    instructionsList.push_back(instructions);
    parseFormat(mRemainStr, instructions);
    instructionsList.push_back(instructions);

        
    return 0;
}

void Parser::parseVM()
{
    /*
    instructions
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

        if (spaceAccount>1 || mInStr[i]=='|')
            continue;
        else if (mInStr[i]=='\t')
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

void Parser::parseAmPm(string &words, string &instructions)
{
    if (words.length()==0)
        return;
    
    int pos=0;
    while ( (pos=words.find("am", pos))!=string::npos )
    {
        //一个合格的am，前后应该只有空格或者数字
        if ((pos==0 || isNumChar(words[pos-1]) || words[pos-1]==' ') &&
           (words[pos+2]=='\0' || isNumChar(words[pos+2]) || words[pos+2]==' '))
        {
            mVmStr.append("|am");
            words = words.replace(pos, 2, " ");
        }
        else
            pos++;
    }

    pos=0;
    while ( (pos=words.find("pm", pos))!=string::npos )
    {
        //一个合格的pm，前后应该只有空格或者数字
        if ((pos==0 || isNumChar(words[pos-1]) || words[pos-1]==' ') &&
           (words[pos+2]=='\0' || isNumChar(words[pos+2]) || words[pos+2]==' '))
        {
            mVmStr.append("|pm");
        }
        else
            pos++;
    }
    removeMultipleSpaces(words);
}

int32_t Parser::parseSampleUnit(const string words, string &instructions)
{
    if (words.length()==0)
        return -1;
    
    /*一个合格的单位应该只有字母*/
    int32_t unitBegin = 0;
    string unit = getWord(words);
    if (unit.length()==0)
        return -1;
    
    while (!isLetterOnly(unit))
    {
        unitBegin = nextWord(words, unitBegin);
        if (unitBegin == string::npos)
            return -1;
        unit = getWord( words.substr(unitBegin, words.length()-unitBegin) );
        if (unit.length()==0)
            return -1;
    }
    
    int32_t numberBegin = 0;
    int32_t number=-1;
    while (numberBegin<words.length())
    {
        if (isNumChar(words[numberBegin]))
            break;
            
        numberBegin++;
    }
    if (numberBegin<unitBegin && numberBegin<words.length())
        number = atoi(words.c_str()+numberBegin);
    
    int32_t unitEnd=unitBegin;
    while(unitEnd < words.length())
    {
        if (isLetter(words[unitEnd]))
            unitEnd++;
        else
            break;
    }
    
    
    if (unit.find("year")!=string::npos)
    {
        instructions.append("|yy");
        if (number>=0)
            instructions.append(std::to_string(number));
        return unitEnd;
    }
    
    if (unit.find("month")!=string::npos)
    {
        instructions.append("|mo");
        if (number>=0)
            instructions.append(std::to_string(number));
        return unitEnd;
    }
    for (size_t i = 0; i < 12; i++)
    {
        if (unit.find(monthShortName[i])!=string::npos)
        {
            instructions.append("|mo");
            instructions.append(std::to_string(i));
            if (number>0)
            {
                instructions.append("|md");
                instructions.append(std::to_string(number));
            }
            return unitEnd;
        }
    }
    
    if (unit.find("week")!=string::npos)
    {
        instructions.append("|ww");
        instructions.append(std::to_string(number));
        return unitEnd;
    }
    for (size_t i = 0; i < 7; i++)
    {
        if (unit.find(weekShortName[i])!=string::npos)
        {
            instructions.append("|wd");
            instructions.append(std::to_string(i));
            return unitEnd;
        }
    }

    if (unit.find("hours"))
    {
        instructions.append("|hh");
        instructions.append(std::to_string(number));
        return unitEnd;
    }
    if (unit.find("morning")!=string::npos)
    {
        instructions.append("|hn9");
        return unitEnd;
    }
    if (unit.find("noon")!=string::npos)
    {
        instructions.append("|hn12");
        return unitEnd;
    }
    if (unit.find("afternoon")!=string::npos)
    {
        instructions.append("|hn14");
        return unitEnd;
    }
    if (unit.find("night")!=string::npos)
    {
        instructions.append("|hn21");
        return unitEnd;
    }
    if (unit.find("mid")!=string::npos)
    {
        instructions.append("|hn24");
        return unitEnd;
    }

    if (unit.find("minute"))
    {
        instructions.append("|mi");
        instructions.append(std::to_string(number));
        return unitEnd;
    }

    if (unit.find("second"))
    {
        instructions.append("|si");
        instructions.append(std::to_string(number));
        return unitEnd;
    }
    
    return unitEnd;
}

void Parser::parseFormat(string &words, string &instructions)
{
    if (words.length()==0)
        return;
    smatch regexResult;

    regex pattern = regex("(\\d{4})[^:]{1}(\\d{1,2})[^:]{1}(\\d{1,2})");
    if (regex_match(words, regexResult, pattern) &&
        atoi(regexResult.str(2).c_str())<=12 &&
        atoi(regexResult.str(3).c_str())<=31);
    {
        instructions.append("|yy");
        instructions.append(regexResult[1]);
        instructions.append("|mo");
        instructions.append(regexResult[2]);
        instructions.append("|md");
        instructions.append(regexResult[3]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{1,2})[^:]{1}(\\d{1,2})[^:]{1}(\\d{4})");
    if (regex_match(words, regexResult, pattern) &&
        atoi(regexResult.str(2).c_str())<=12 &&
        atoi(regexResult.str(3).c_str())<=31);
    {
        instructions.append("|md");
        instructions.append(regexResult[1]);
        instructions.append("|mo");
        instructions.append(regexResult[2]);
        instructions.append("|yy");
        instructions.append(regexResult[3]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{4})[^:]{1}(\\d{1,2})");
    if (regex_match(words, regexResult, pattern) &&
        atoi(regexResult.str(2).c_str())<=31);
    {
        instructions.append("|yy");
        instructions.append(regexResult[1]);
        instructions.append("|mo");
        instructions.append(regexResult[2]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    if (regex_match(words, regexResult, pattern) &&
        atoi(regexResult.str(1).c_str())<=24 && 
        atoi(regexResult.str(2).c_str())<=60 &&
        atoi(regexResult.str(3).c_str())<=60);
    {
        instructions.append("|hh");
        instructions.append(regexResult[1]);
        instructions.append("|mi");
        instructions.append(regexResult[2]);
        instructions.append("|ss");
        instructions.append(regexResult[3]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{1,2}):(\\d{1,2})");
    if (regex_match(words, regexResult, pattern) &&
        atoi(regexResult.str(1).c_str())<=24 && 
        atoi(regexResult.str(2).c_str())<=60);
    {
        instructions.append("|hh");
        instructions.append(regexResult[1]);
        instructions.append("|mi");
        instructions.append(regexResult[2]);
        regex_replace(words, pattern, "");
    }
}

void Parser::removeMultipleSpaces(string &str){
    uint32_t i=0;
    uint32_t gotSpace=0;
    while (i<str.length())
    {
        if (str[i]==' ')
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