#include <stdint.h>
#include <string>

using namespace std;

#include "cmdline.h"
#include "debug_print.h"
#include "JrmeConfig.h"
#include "WriteMode.h"

int main(int argc, char* argv[]) {
    if (!JrmeConfig::installIfNeed())
    {
        JLOGE("[E] install jrme fail!!\n");
        return -1;
    }
    
    cmdline::parser cmd;
    cmd.add<string>("title", 'T', "The title of journal", false, "");
    cmd.add<string>("at", 'A', "the time of journal", false, "");
    cmd.add<string>("content", 'C', "The content of journal", false, "");

    cmd.parse_check(argc, argv);
    
    string journalBookPath = JrmeConfig::getDefaultJournalBookPath();
    
    if (cmd.exist("title") || cmd.exist("at") || cmd.exist("content"))
    {
        journlWriteMode(journalBookPath, cmd.get<string>("at"), cmd.get<string>("title"), cmd.get<string>("content"));
        return 0;
    }


    journlWriteMode(journalBookPath, string(""), string(""), string(""));
    return 0;
}