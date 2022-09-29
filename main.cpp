#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <stdlib.h>

#include "src/ArgumentHandler.hpp"
#include "src/DownloadManager.hpp"

int main(int argc, char** argv) {
    ArgumentHandler aH;
    int isHelp = aH.GetOptions(argc, argv);
    if (isHelp) std::exit(0);

    DownloadManager dM = DownloadManager(aH.tempPath, aH.downloadPath, aH.range);
    dM.Begin();
    
    return 0;
}
