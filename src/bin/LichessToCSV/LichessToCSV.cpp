#include "pgn_parsing/MatchParsing.hpp"
#include "pgn_parsing/types/MatchInfo.hpp"
#include "pgn_parsing/types/MatchResult.hpp"
#include <string>
#include <iostream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "download/DataDownloading.hpp"
#include "utilities/CommandLineUtilities.hpp"
#include "utilities/FileUtilities.hpp"
#include "extraction/FileExtraction.hpp"
#include "ArgumentParsing.hpp"
#include "extraction/FilterGeneration.hpp"

int main(int argc, char** argv) {
    Arguments args = ArgumentParsing::Parse(argc, argv);
    std::vector<Filter::Filter*> filters = FilterGeneration::GenerateFilters(args.minRating, args.maxRating);

    DataDownloading::DownloadData(args.tempPath, args.downloadPath, args.range);
    
    std::ofstream oFile(args.csvPath);
    oFile << "outcome,moves" << std::endl;
    Utilities::DirIterator(args.downloadPath, "zst", [&](std::string filePath){
        FileExtraction::ExtractPGNFile(filePath, filters, [&](MatchInfo match){
            oFile << MatchResults[(uint) match.result] << "," << match.moves << std::endl;
        });
    });
    oFile.close();
   
    return 0;
}

