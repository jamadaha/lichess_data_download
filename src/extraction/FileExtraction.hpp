#ifndef FILE_EXTRACTOR
#define FILE_EXTRACTOR

#include <cstdint>
#include <stdexcept>
#include <string>
#include <functional>
#include <iostream>
#include <vector>

#include "Filter.hpp"
#include "pgn_parsing/types/MatchInfo.hpp"
#include "utilities/CommandLineUtilities.hpp"
#include "pgn_parsing/MatchParsing.hpp"

namespace FileExtraction {
    static inline const uint BUFFER_LIMIT = 100000;

    static void ExtractFile(std::string filePath, std::function<void(std::string)> lineCallback) {
        char buffer[BUFFER_LIMIT];

        FILE *stream = popen(std::string("zstdcat ").append(filePath).c_str(), "r");
        if (stream == nullptr)
            throw std::logic_error("Unable to open file: " + filePath);

        std::cout << Utilities::BoldOn << "Extracting " << Utilities::BoldOff << filePath << std::endl;
        
        while(fgets(buffer, BUFFER_LIMIT, stream) != nullptr)
            lineCallback(buffer);

        pclose(stream);
    }

    static void ExtractPGNFile(std::string filePath, const std::vector<Filter::Filter*> &filters, std::function<void(MatchInfo)> matchCallback) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        uint64_t gameCount = 0;
        uint64_t purgedCount = 0;
        bool encounteredNL = false;
        std::vector<std::string> matchLines;
        FileExtraction::ExtractFile(filePath, [&](std::string line){
            if (line == "\n") {
                if (!encounteredNL)
                    encounteredNL = true;
                else {
                    MatchInfo match = MatchParsing::ParseMatch(matchLines);
                    ++gameCount;
                    encounteredNL = false;
                    matchLines.clear();
                    for (const auto &filter : filters)
                        if (!(*filter)(match)) {
                            ++purgedCount;
                            return;
                        }
                    matchCallback(match);
                }
            }
            matchLines.push_back(line);
        });
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::cout << "Extracted " << gameCount << " games in " << timeTaken << "ms (" << gameCount / timeTaken << " games/ms)" << std::endl;
        std::cout << "Removed " << purgedCount << " games because of filters " << ((double) purgedCount / (double) gameCount) * 100 << "%" << std::endl;
        std::cout << "Outputting " << gameCount - purgedCount << " games" << std::endl;
    }
};

#endif

