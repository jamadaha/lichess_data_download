#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <stdlib.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "IO/ArgumentParsing.hpp"
#include "download/DataDownloading.hpp"

int main(int argc, char** argv) {
    Arguments args = ArgumentParsing::Parse(argc, argv);

    DataDownloading::DownloadData(args.tempPath, args.downloadPath, args.range);

    return 0;
}
