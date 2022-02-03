#include <memory>
#include <stdio.h>
#include <stdint.h>

#include <string>

using namespace std;

#include "Journal.h"
#include "TxtJournalIO.h"
#include "JournalIOBase.h"

int main(int argc, char* argv[]) {
    bool ret;

    shared_ptr<JournalIOBase> jIO(new TxtJournalIO());
    return 0;
}