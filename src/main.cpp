#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <filesystem>
#include <stdio.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "ArgumentParsing.hpp"
#include "download/DataDownloading.hpp"

int main(int argc, char** argv) {
    Arguments args = ArgumentParsing::Parse(argc, argv);

    DataDownloading::DownloadData(args.tempPath, args.downloadPath, args.range);
        
    const std::filesystem::path downloadPath(args.downloadPath);
    for (const auto &entry : std::filesystem::directory_iterator(downloadPath)) {
        const auto entryPath = entry.path();
        // Ignore unfinished downloads
        if (entryPath.filename().string().find(".temp") != std::string::npos)
            continue;
        printf("Extracting %s\n", entryPath.c_str());

        const uint BUFFER_LIMIT = 4096;
        char buffer[BUFFER_LIMIT]{' '};
        FILE *stream = popen(std::string("zstdcat ").append(entryPath).c_str(), "r");
        if (stream == NULL)
            throw std::logic_error("Unable to open file");
        while(fgets(buffer, BUFFER_LIMIT, stream) != NULL) {
        }
        pclose(stream);
    }
    

    return 0;
}
