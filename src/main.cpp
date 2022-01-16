#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <string>

using namespace std;

#include "KiloEditor.h"

int main(int argc, char* argv[]) {
    printf("hello jrme \r\n");
    TxtEditor editor;
    editor.setInitStr(string("==============jrnal begin=============\nconfig\n========================================"));
    string gotStr;
    bool wantSave =editor.getTextFromEditor(gotStr);
    system("clear");
    if (wantSave == true)
    {
        printf("user want to save\n");
        printf("get string:\n%s",gotStr.c_str());
    }
    else
    {
        printf("user don't want to save\n");
    }
    return 0;
}