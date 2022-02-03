#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <string>

using namespace std;

#include "KiloEditor.h"
#include "JrmeInstall.h"

int main(int argc, char* argv[]) {
    bool ret;
    ret = installIfNeed();
    printf("installIfNeed ret=%d\n", ret);
    return 0;
}