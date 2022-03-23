#include <stdint.h>
#include <string>

using namespace std;

#include "debug_print.h"
#include "cmdline.h"
#include "WriteMode.h"
#include "JrmeConfig.h"

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
    
    INI::File configFile = INI::File(JrmeConfig::getConfigFilePath());
    string journalBookPath = configFile.GetSection("journal books")->GetValue("default").AsString();
    
    if (cmd.exist("title") || cmd.exist("at") || cmd.exist("content"))
    {
        journlWriteMode(journalBookPath, cmd.get<string>("at"), cmd.get<string>("title"), cmd.get<string>("content"));
        return 0;
    }


    journlWriteMode(journalBookPath, string(""), string(""), string(""));
    return 0;
}