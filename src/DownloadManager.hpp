#ifndef DOWNLOAD_MANAGER
#define DOWNLOAD_MANAGER

#include <string>
#include <iostream>
#include "Download.hpp"
#include "Downloader.hpp"
#include "DownloadParser.hpp"
#include "Utilities.hpp"
#include "../include/indicators/indicators.hpp"

class DownloadManager {
public:
    DownloadManager(std::string tempPath, std::string downloadPath, std::string range);
    ~DownloadManager();
    void Begin();

private:
    std::string tempPath;
    std::string downloadPath;
    std::string range;
    Downloader downloader;
};

#endif
