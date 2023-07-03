#include "pgn_parsing/MatchParsing.hpp"
#include "pgn_parsing/types/MatchInfo.hpp"
#include "pgn_parsing/types/MatchResult.hpp"
#include <string>
#include <iostream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "ArgumentParsing.hpp"
#include "download/DataDownloading.hpp"
#include "utilities/CommandLineUtilities.hpp"
#include "utilities/FileUtilities.hpp"
#include "extraction/FileExtraction.hpp"
#include "db/postgres/PostgresConnection.hpp"

int main(int argc, char** argv) {
    Arguments args = ArgumentParsing::Parse(argc, argv);

    DataDownloading::DownloadData(args.tempPath, args.downloadPath, args.range);
  
    PostgresConnection conn{args.host, args.port, args.dbName, args.password};
    Utilities::DirIterator(args.downloadPath, "zst", [&](std::string filePath){
        conn.BeginTransaction();
        FileExtraction::ExtractPGNFile(filePath, {}, [&](MatchInfo match){
            conn.InsertMatch(match);
        });
        conn.CommitTransaction();
    });
   
    return 0;
}

