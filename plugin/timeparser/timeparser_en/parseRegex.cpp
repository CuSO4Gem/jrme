#include <algorithm>
#include "date.h"
#include <regex>
#include <stdio.h>
#include <time.h>
#include "timeparser_plugin.h"
#include "parseRegex.h"

using namespace ec;
using namespace std;

#define DEBUG
#ifdef DEBUG
#define DEBUGP(...) (printf(__VA_ARGS__))
#else
#define DEBUGP(info)
#endif

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

typedef struct vmCmd {
  char cmd[2];
  bool haveNumber;
  int32_t number;
} vmCmd_t;

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
        if (str[pos]==' ')
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

void eraseWrod(string &str)
{
    size_t pos = nextWord(str, 0);
    if(pos==string::npos)
        str.clear();
    else if (pos>=0)
        str.erase(0, pos);
    else
        return;
}

class Parser
{
public:
    Parser(const char *intStr);
    int getTime(ptm timeGot);

private:
    int32_t mEstimation;
    string mInStr;
    string mRemainStr;
    string mVmStr;
    Date mTargetTime;
    uint32_t mParseFlag;
    
    vmCmd_t readVMCmd(string &instructions);
    void parseVM();
    void prepare();
    void parseAmPm(string &words, string &instructions);
    size_t parseSampleUnit(const string words, string &instructions);
    void parseFormat(string &words, string &instructions);
    void parsePhrase(string &words, string &instructions);

    void removeMultipleSpaces(string &str);
};

vmCmd_t Parser::readVMCmd(string &instructions)
{
    vmCmd_t cmd;
    memset(&cmd, 0, sizeof(cmd));
    
    size_t pos;
    if ((pos = instructions.find("|"))==string::npos)
        return cmd;
    if (instructions.length()-pos < 3)
        return cmd;
    pos++;
    cmd.cmd[0] = instructions[pos];
    pos++;
    cmd.cmd[1] = instructions[pos];
    pos++;

    if (!isNumChar(instructions[pos]))
    {
        instructions.erase(0, pos+1);
        return cmd;
    }
    else
    {
        cmd.haveNumber = true;
        cmd.number = atoi(&instructions[pos]);
        while (pos<instructions.length() && isNumChar(instructions[pos]))
            pos++;
        
        if (pos<instructions.length() && instructions[pos]=='-')
        {
            cmd.number = -abs(cmd.number);
            pos++;
        }
        instructions.erase(0, pos+1);
        return cmd;
    }
}

Parser::Parser(const char *intStr)
{
    mEstimation = 5;
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
    #ifdef DEBUG
        printf("inst=");
        for (auto &it:instructionsList)
        {
            printf("%s",it.c_str());
        }
        printf("\r\n");
    #endif

    DEBUGP("parseAmPm\r\n");
    instructions.clear();
    parseAmPm(mRemainStr, instructions);
    instructionsList.push_back(instructions);
    DEBUGP("parsePhrase\r\n");
    instructions.clear();
    parseFormat(mRemainStr, instructions);
    instructionsList.push_back(instructions);
    DEBUGP("parsePhrase\r\n");
    instructions.clear();
    parsePhrase(mRemainStr, instructions);
    instructionsList.push_back(instructions);
    
    string word;
    bool wordLastFlag = false;
    DEBUGP("while start\r\n");
    while ((word=getWord(mRemainStr)).length() != 0)
    {
        DEBUGP("mRemainStr=%s\r\n",mRemainStr.c_str());
#ifdef DEBUG
        printf("inst=");
        for (auto &it:instructionsList)
        {
            printf("%s",it.c_str());
        }
        printf("\r\n");
#endif
        smatch regexResult;
        bool haveNumber = false;
        bool haveLetter = false;
        bool haveOtherChar = false;

        for (size_t i = 0; i < word.length(); i++)
        {
            if (isNumChar(word[i]))
                haveNumber = true;
            else if (isLetter(word[i]))
                haveLetter = true;
            else
            {
                haveOtherChar = true;
                break;
            }
        }
        
        if (word==string("last"))
        {
            wordLastFlag = true;
            eraseWrod(mRemainStr);
            continue;
        }
        /* 4 number, maybe year*/
        if (haveLetter && haveNumber && !haveOtherChar && word.length()==4)
        {
            /* can not "last yyyy" in sentence*/
            if (wordLastFlag)
            {
                wordLastFlag = false;
                mEstimation = min(mEstimation, TIME_PARSE_PARTLY_SUCCESS);
            }
            instructions = string("|yy"); 
            instructions.append(word);
            instructionsList.push_back(instructions);
            eraseWrod(mRemainStr);
            continue;
        }
        if(word==string("today"))
        {
            /* can not "last today" in sentence*/
            if (wordLastFlag)
            {
                wordLastFlag = false;
                mEstimation = min(mEstimation, TIME_PARSE_PARTLY_SUCCESS);
            }
            instructions = string("|dd");
            instructionsList.push_back(instructions);
            eraseWrod(mRemainStr);
            continue;
        }
        if(word==string("yesterday"))
        {
            /* can not "last yesterday" in sentence*/
            if (wordLastFlag)
            {
                wordLastFlag = false;
                mEstimation = min(mEstimation, TIME_PARSE_PARTLY_SUCCESS);
            }
            instructions = string("|dd1-");
            instructionsList.push_back(instructions);
            eraseWrod(mRemainStr);
            continue;
        }
        if(regex_search(word, regexResult, regex("(\\d{1,2})(st|nd|th)")))
        {
            instructions = string("|md");
            instructions.append(regexResult.str(1));
            instructionsList.push_back(instructions);
            eraseWrod(mRemainStr);
            continue;
        }
        if(!haveLetter && haveNumber && !haveOtherChar)
        {
            instructions.clear();
            size_t readSize = parseSampleUnit(mRemainStr, instructions);
            if (readSize>0)
            {
                mRemainStr.erase(0, readSize);
                if (wordLastFlag)
                {
                    wordLastFlag = false;
                    vmCmd_t cmd = readVMCmd(instructions);
                    
                    if (cmd.cmd[0] && cmd.cmd[1])
                    {
                        string instTmp = string("|");
                        instTmp.append(string(cmd.cmd));
                        if (cmd.haveNumber)
                            instTmp.append(std::to_string(abs(cmd.number)));
                        instTmp.append(string("-"));
                        instTmp.append(instructions);
                        instructionsList.push_back(instTmp);
                    }
                }
                continue;
            }
        }
        DEBUGP("wile end\r\n");
        eraseWrod(mRemainStr);
    }
    
    instructions.clear();
    for (auto &it:instructionsList)
    {
        instructions.append(it);
    }
    instructionsList.clear();

    DEBUGP("get instructions:%s\r\n",instructions.c_str());
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
    dd: day
    ww:week
    wd:day of month
    wn: week end
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
            mVmStr.append(string("|am"));
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
            mVmStr.append(string("|pm"));
        }
        else
            pos++;
    }
    removeMultipleSpaces(words);
}

size_t Parser::parseSampleUnit(const string words, string &instructions)
{
    if (words.length()==0)
        return -1;
    
    /*一个合格的单位应该只有字母*/
    size_t unitBegin = 0;
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
    size_t unitEnd=unitBegin;
    while(unitEnd < words.length())
    {
        if (isLetter(words[unitEnd]))
            unitEnd++;
        else
            break;
    }
    
    size_t numberBegin = 0;
    size_t number=-1;
    while (numberBegin<words.length())
    {
        if (isNumChar(words[numberBegin]))
            break;
            
        numberBegin++;
    }
    if (numberBegin<unitBegin && numberBegin<words.length())
        number = atoi(words.c_str()+numberBegin); 
    
    if (unit==string("year") || unit==string("years"))
    {
        instructions.append(string("|yy"));
        if (number>=0)
            instructions.append(std::to_string(number));
        return unitEnd;
    }
    
    if (unit==string("month") || unit==string("month"))
    {
        instructions.append(string("|mo"));
        if (number>=0)
            instructions.append(std::to_string(number));
        return unitEnd;
    }
    for (size_t i = 0; i < 12; i++)
    {
        if (unit.find(monthShortName[i])!=string::npos)
        {
            instructions.append(string("|mo"));
            instructions.append(std::to_string(i));
            if (number>0)
            {
                instructions.append(string("|md"));
                instructions.append(std::to_string(number));
            }
            return unitEnd;
        }
    }

    if (unit==string("weekend"))
    {
        instructions.append(string("|wn"));
        return unitEnd;
    }
    
    if (unit==string("week"))
    {
        instructions.append(string("|ww"));
        instructions.append(std::to_string(number));
        return unitEnd;
    }
    for (size_t i = 0; i < 7; i++)
    {
        if (unit.find(weekShortName[i])!=string::npos)
        {
            instructions.append(string("|wd"));
            instructions.append(std::to_string(i));
            return unitEnd;
        }
    }
    if (unit==string("day") || unit==string("days"))
    {
        instructions.append(string("|dd"));
        instructions.append(std::to_string(number));
        return unitEnd;
    }
    if (unit==string("morning"))
    {
        instructions.append(string("|hn9"));
        return unitEnd;
    }
    if (unit==string("afternoon"))
    {
        instructions.append(string("|hn14"));
        return unitEnd;
    }
    if (unit==string("noon"))
    {
        instructions.append(string("|hn12"));
        return unitEnd;
    }
    if (unit==string("night"))
    {
        instructions.append(string("|hn21"));
        return unitEnd;
    }
    if (unit==string("midnight"))
    {
        instructions.append(string("|hn24"));
        return unitEnd;
    }
    if (unit==string("hour") || unit==string("hours"))
    {
        instructions.append(string("|hh"));
        instructions.append(std::to_string(number));
        return unitEnd;
    }
    if (unit==string("minute") || unit==string("minutes"))
    {
        instructions.append(string("|mi"));
        instructions.append(std::to_string(number));
        return unitEnd;
    }

    if (unit==string("second") || unit==string("seconds"))
    {
        instructions.append(string("|si"));
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

    regex pattern = regex("(\\d{4})[^:]{1,1}(\\d{1,2})[^:]{1,1}(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) &&
        atoi(regexResult.str(2).c_str())<=12 &&
        atoi(regexResult.str(3).c_str())<=31)
    {
        instructions.append(string("|yy"));
        instructions.append(regexResult[1]);
        instructions.append(string("|mo"));
        instructions.append(regexResult[2]);
        instructions.append(string("|md"));
        instructions.append(regexResult[3]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{1,2})[^:]{1}(\\d{1,2})[^:]{1}(\\d{4})");
    if (regex_search(words, regexResult, pattern) &&
        atoi(regexResult.str(2).c_str())<=12 &&
        atoi(regexResult.str(3).c_str())<=31)
    {
        instructions.append(string("|md"));
        instructions.append(regexResult[1]);
        instructions.append(string("|mo"));
        instructions.append(regexResult[2]);
        instructions.append(string("|yy"));
        instructions.append(regexResult[3]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{4})[^:]{1}(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) &&
        atoi(regexResult.str(2).c_str())<=31)
    {
        instructions.append(string("|yy"));
        instructions.append(regexResult[1]);
        instructions.append(string("|mo"));
        instructions.append(regexResult[2]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) &&
        atoi(regexResult.str(1).c_str())<=24 && 
        atoi(regexResult.str(2).c_str())<=60 &&
        atoi(regexResult.str(3).c_str())<=60)
    {
        instructions.append(string("|hh"));
        instructions.append(regexResult[1]);
        instructions.append(string("|mi"));
        instructions.append(regexResult[2]);
        instructions.append(string("|ss"));
        instructions.append(regexResult[3]);
        regex_replace(words, pattern, "");
    }

    pattern = regex("(\\d{1,2}):(\\d{1,2})");
    if (regex_search(words, regexResult, pattern) &&
        atoi(regexResult.str(1).c_str())<=24 && 
        atoi(regexResult.str(2).c_str())<=60)
    {
        instructions.append(string("|hh"));
        instructions.append(regexResult[1]);
        instructions.append(string("|mi"));
        instructions.append(regexResult[2]);
        regex_replace(words, pattern, "");
    }
    removeMultipleSpaces(words);
}

void Parser::parsePhrase(string &words, string &instructions)
{
    string strFind;
    strFind = string("the year before last year");
    if(words.find(strFind)!=string::npos)
    {
        words.replace(words.find(strFind), strFind.length(), "");
        instructions.append(string("|yy2-"));
    }

    strFind = string("the month before last month");
    if(words.find(strFind)!=string::npos)
    {
        words.replace(words.find(strFind), strFind.length(), "");
        instructions.append(string("|mo2-"));
    }

    strFind = string("the day before yesterday");
    if(words.find(strFind)!=string::npos)
    {
        words.replace(words.find(strFind), strFind.length(), "");
        instructions.append(string("|md2-"));
    }

    removeMultipleSpaces(words);
    return;
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