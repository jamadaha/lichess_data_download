#ifndef DOWNLOAD_PARSER
#define DOWNLOAD_PARSER

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>

#include "Download.hpp"

class DownloadParser {
public:
    static std::vector<Download> GetDownloads(std::string path) {
        Downloader downloader = Downloader();
        std::string downloadListPath = path + "/download_list.txt";
        downloader.AddDownload("https://database.lichess.org/standard/list.txt", downloadListPath);

        downloader.LoadNextDownload();
        std::cout << "Getting possible downloads... ";
        while(downloader.Update()) { sleep(1); }
        std::cout << "Finished\n";
        return ParseDownloadFile(downloadListPath);
    };

private:
    static std::vector<Download> ParseDownloadFile(std::string filePath) {
        std::vector<Download> downloads;
        std::ifstream file(filePath);

        const int bufferSize = 256;
        char line[bufferSize];

        while (file.getline(line, bufferSize)) {
            std::string sLine = line;
            sLine.erase(remove_if(sLine.begin(), sLine.end(), ::isspace), sLine.end()); // Remove whitespace
            std::string link = sLine;
            int year;
            int month;
            
            while (!std::isdigit(sLine[0])) // Find first number - Assumes first number is year
                sLine = sLine.substr(1);
            year = std::atoi(sLine.substr(0, 4).c_str()); // Get year as number - Assumes 4 digit year
            sLine = sLine.substr(5); // Remove year and seperator between year and month
            month = std::atoi(sLine.substr(0, 2).c_str()); // Get month as number - Assumes 2 digit month
                
            downloads.push_back(Download(year, month, link));
        }

        return downloads;
    }
};

#endif
