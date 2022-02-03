#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include "WriteMode.h"
#include "JrmeConfig.h"

int main(int argc, char* argv[]) {
    bool ret;
    ret = installIfNeed();

    INI::File configFile = INI::File(getConfigPath());
    string journalBookPath = configFile.GetSection("journal books")->GetValue("default").AsString();
    string title;
    for (size_t i = 1; i < argc; i++)
    {
        if (i>1)
            title.append(string(" ") + argv[i]);
        else
            title.append(argv[i]);
    }
    writeJournal(journalBookPath, string(""), title);
    
    return 0;
}