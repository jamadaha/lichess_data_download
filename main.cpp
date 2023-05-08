#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <stdlib.h>

#include "src/ArgumentParser.hpp"
#include "src/DownloadManager.hpp"

int main(int argc, char** argv) {
    Arguments args = ArgumentParser::Parse(argc, argv);
    DownloadManager dM = DownloadManager(args.tempPath, args.downloadPath, args.range);
    dM.Begin();
    
    return 0;
}
