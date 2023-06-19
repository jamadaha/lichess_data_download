#ifndef DOWNLOAD_PARSING
#define DOWNLOAD_PARSING

#include <exception>
#include <fstream>

#include "doctest/doctest.h"
#include "types/Download.hpp"
#include "types/DownloadList.hpp"

namespace DownloadParsing {
    // Link to a file that stores available downloads from lichess
    // Each line in the file states a month of data from the site
    static inline std::string AVAILABLE_DOWNLOAD_LINK = "https://database.lichess.org/standard/list.txt";
    
    // Converts a lichess file link to a download
    // TODO: Improve detection of format changes
    static Download ParseDownload(std::string line) {
        uint yearIndex = 0;
        try {
            // Find first number - Assumes first number is year
            while (!std::isdigit(line[yearIndex++])) {} 
        } catch (...) {
            printf("Error in parsing. Likely due to changed link format.\n");
            exit(1);
        }

        // Get year as number - Assumes 4 digit year
        uint year = std::atoi(line.substr(yearIndex - 1, 4).c_str());  
        // Get month as number - Assumes 2 digit month
        uint month = std::atoi(line.substr(yearIndex + 4, 2).c_str());

        return Download(year, month, line);
    }

    static DownloadList ParseDownloads(const std::vector<std::string> &lines) {
        DownloadList downloads;

        for (auto line : lines)
            downloads.push_back(ParseDownload(line));

        return downloads;
    };

    static DownloadList ParseDownloadFile(std::string path) {
        std::vector<std::string> lines;

        std::fstream fStream;
        fStream.open(path);

        std::string line;
        while (fStream >> line)
            lines.push_back(line);

        fStream.close();

        return ParseDownloads(lines);
    };

    namespace Test {
        TEST_CASE("Parse download link") {
            const std::string testLink = 
                "https://database.lichess.org/standard/lichess_db_standard_rated_2013-01.pgn.zst";
            
            const auto download = ParseDownload(testLink);
            
            CHECK_EQ(2013, download.year);
            CHECK_EQ(01, download.month);
            CHECK_EQ(testLink, download.link);
        }

        TEST_CASE("Parse multiple") {
            const std::vector<std::string> links = {
                "https://database.lichess.org/standard/lichess_db_standard_rated_2013-01.pgn.zst",
                "https://database.lichess.org/standard/lichess_db_standard_rated_2014-02.pgn.zst"
            };
            
            const auto downloads = ParseDownloads(links);
            
            CHECK_EQ(2, downloads.size());
            CHECK_EQ(2013, downloads[0].year);
            CHECK_EQ(01, downloads[0].month);
            CHECK_EQ(links[0], downloads[0].link);
            CHECK_EQ(2014, downloads[1].year);
            CHECK_EQ(02, downloads[1].month);
            CHECK_EQ(links[1], downloads[1].link);
        }
    }
}

#endif

