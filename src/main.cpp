#include "parsing/types/MatchInfo.hpp"
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
#include "parsing/MatchParsing.hpp"

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

        const uint BUFFER_LIMIT = 16384;
        char buffers[40][BUFFER_LIMIT];
        uint index = 0;
        FILE *stream = popen(std::string("zstdcat ").append(entryPath).c_str(), "r");
        if (stream == nullptr)
            throw std::logic_error("Unable to open file");
        while(fgets(buffers[index++], BUFFER_LIMIT, stream) != nullptr) {
            if (index > 0 && buffers[index - 1][0] == '1') {
                std::vector<std::string> lines;
                for (uint i = 0; i < index; ++i)
                    if (buffers[i][0] != '\n')
                        lines.push_back(buffers[i]);

                std::optional<MatchInfo> info;
                try {
                    info = MatchParsing::ParseMatch(lines);
                } catch (const std::exception& ex) {
                    printf("%s\n", ex.what());
                    exit(1);
                } catch (...) {
                    printf("Unhandled exception\n");
                }
                index = 0;
            }
        }
        printf("Finished Extraction\n");
        pclose(stream);
    }
    

    return 0;
}
