#include "parsing/types/MatchInfo.hpp"
#include <cstdint>
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
#include <pqxx/pqxx>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "ArgumentParsing.hpp"
#include "download/DataDownloading.hpp"
#include "parsing/MatchParsing.hpp"

int main(int argc, char** argv) {
    Arguments args = ArgumentParsing::Parse(argc, argv);

    DataDownloading::DownloadData(args.tempPath, args.downloadPath, args.range);
        
    std::cout << Utilities::BoldOn << "----BEGINNING EXTRACTION----" << Utilities::BoldOff << std::endl;
    pqxx::connection conn("host=" + args.host + " port=" + args.port + " dbname=" + args.dbName + " password=" + args.password);
    std::cout << "Connected to " << conn.dbname() << std::endl;
    pqxx::work tx(conn);
    
    const std::filesystem::path downloadPath(args.downloadPath);
    for (const auto &entry : std::filesystem::directory_iterator(downloadPath)) {
        const auto entryPath = entry.path();
        // Ignore unfinished downloads
        if (entryPath.filename().string().find(".temp") != std::string::npos)
            continue;
        std::cout << Utilities::BoldOn << "Extracting " << Utilities::BoldOff << entryPath.filename() << std::endl;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        uint64_t gameCount = 0;
        const uint BUFFER_LIMIT = 16384;
        char buffers[40][BUFFER_LIMIT];
        uint index = 0;
        FILE *stream = popen(std::string("zstdcat ").append(entryPath).c_str(), "r");
        if (stream == nullptr)
            throw std::logic_error("Unable to open file");
        while(fgets(buffers[index++], BUFFER_LIMIT, stream) != nullptr) {
            if (index > 0 && 
                    (buffers[index - 1][0] == '1' || buffers[index - 1][0] == '0' ||
                     buffers[index-1][0] == ' ')) {
                std::vector<std::string> lines;
                for (uint i = 0; i < index; ++i)
                    if (buffers[i][0] != '\n')
                        lines.push_back(buffers[i]);
                MatchInfo info = MatchParsing::ParseMatch(lines);
                tx.exec0("INSERT INTO \"games\"(white_player, black_player) VALUES ('" + info.whitePlayer + "', '" + info.blackPlayer + "');");

                index = 0;
                ++gameCount;
            }
        }
        pclose(stream);
        tx.commit();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::cout << "Inserted " << gameCount << " games in " << timeTaken << "ms (" << gameCount / timeTaken << " games/ms)" << std::endl;
    }
    
    std::cout << Utilities::BoldOn << "----FINISHED  EXTRACTION----" << Utilities::BoldOff << std::endl;

    return 0;
}
