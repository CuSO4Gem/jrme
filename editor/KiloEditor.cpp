extern "C"{
#include "editorControl.h"
#include "editorUtilities.h"
#include "rawMode.h"
#include "rowUtilities.h"
#include "search.h"
#include "editor_struct.h"
}

#include <stdint.h>
#include <stdbool.h>
#include <string>

#include "KiloEditor.h"

TxtEditor::TxtEditor(string initStr)
{
    mInitStr = initStr;
}

TxtEditor::~TxtEditor()
{
    ;
}

void TxtEditor::setInitStr(string initStr)
{
    mInitStr = initStr;
}

void TxtEditor::clearInitStr()
{
    mInitStr = "";
}


bool TxtEditor::getTextFromEditor(string &strGot)
{
    enableRawMode();
    initEditor();
    if (mInitStr.length()!=0)
    {
        editorFromString(mInitStr.c_str());
    }
    editorSetStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit");

    bool keepLoop = true;
    bool saveString = true;
    while(keepLoop){
        clearScreen(1);
        int curr=readKey();
        switch(curr){//add all the mode controls below
            case '\r':
                editorInsertNewline();
                break;
            case ctrl('q'):
                clearScreen(0);
                saveString = false;
                keepLoop = false;
                break;
            case ctrl('f'):
                search();
                break;
            case HOME_KEY:
                E.cx=0;
                break;
            case END_KEY:
                if(E.cy<E.numRows)
                    E.cx=E.row[E.cy].size;
                break;
            case PAGE_UP:
            case PAGE_DOWN:
                {
                    if (curr == PAGE_UP) {
                        E.cy = E.rowOffset;
                    }
                    else if(curr == PAGE_DOWN){
                        E.cy = E.rowOffset + E.screenRows - 1;
                        if (E.cy > E.numRows)
                            E.cy = E.numRows;
                    }
                    int times = E.screenRows;
                    while (times--)
                        moveCursor(curr == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                }
                break;
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_LEFT:
            case ARROW_RIGHT:
                moveCursor(curr);
                break;
            case BACKSPACE:
            /*ctrl('h') 不能删除，会导致一些终端退格失败*/
            case ctrl('h'):
            case DEL_KEY:
                if(curr== DEL_KEY)
                    moveCursor(ARROW_RIGHT);
                editorDelChar();
                break;
            case ctrl('l'):
            case '\x1b':
                break;
            case ctrl('s'):
                saveString = true;
                keepLoop = false;
                break;
            default:
                editorInsertChar(curr);
                editorSetStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit");
                break;
        }
    }
    disableRawMode();

    if (saveString)
    {
        int len;
        char *textOut=editorRowsToString(&len);
        if (textOut!=NULL)
        {
            strGot = string(textOut);
            free(textOut);
        }
        else
            strGot = string("");
    }
    else
        strGot = string("");
    
    /*free all row*/
    if (E.numRows>0)
    {
        delRow(0);
    }
    
    return saveString;
}