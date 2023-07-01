#ifndef FILE_EXTRACTOR
#define FILE_EXTRACTOR

#include <stdexcept>
#include <string>
#include <functional>
#include <iostream>

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

    static void ExtractPGNFile(std::string filePath, std::function<void(MatchInfo)> matchCallback) {
        bool encounteredNL = false;
        std::vector<std::string> matchLines;
        FileExtraction::ExtractFile(filePath, [&](std::string line){
            if (line == "\n") {
                if (!encounteredNL)
                    encounteredNL = true;
                else {
                    MatchInfo match = MatchParsing::ParseMatch(matchLines);
                    matchCallback(match);
                    encounteredNL = false;
                    matchLines.clear();
                }
            }
            matchLines.push_back(line);
        });
    }
};

#endif

