#include "DownloadManager.hpp"

DownloadManager::DownloadManager(std::string tempPath, std::string downloadPath, std::string range) 
:   tempPath(tempPath),
    downloadPath(downloadPath),
    range(range) {
    downloader = Downloader();
    Downloader::Init();
}

DownloadManager::~DownloadManager() {
    Downloader::Clean();
}

void DownloadManager::Begin() {
    std::cout << Utilities::BoldOn << "----BEGINNING DOWNLOAD----" << Utilities::BoldOff << std::endl;
    
    std::vector<Download> allDownloads = DownloadParser::GetDownloads(tempPath);
    std::vector<Download> rangedDownloads = DownloadParser::GetRange(allDownloads, range);
    Downloader downloader = Downloader();
    for (int i = rangedDownloads.size() - 1; i >= 0; i--) {
        Download download = rangedDownloads[i];
        downloader.AddDownload(download.link, 
        downloadPath + std::to_string(download.year) + "-" + std::to_string(download.month) + ".pgn.bz2", 
        DownloadType::Binary);
    }    using namespace indicators;


    while (downloader.LoadNextDownload()) {
        std::cout << Utilities::BoldOn << "Downloading " << Utilities::BoldOff << downloader.GetDownloadLink() << std::endl;
        ProgressBar bar {
                option::BarWidth{50},
                option::Start{"["},
                option::Fill{"■"},
                option::Lead{"■"},
                option::Remainder{"-"},
                option::End{"]"},
                option::ShowPercentage{true},
                option::ShowElapsedTime{true}
        };
        while (downloader.Update()) {
            double progress = downloader.GetDownloadProgress();
            bar.set_progress(downloader.GetDownloadProgress() * 100);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        bar.set_progress(100);
    }
    std::cout << Utilities::BoldOn << "----FINISHED  DOWNLOAD----" << Utilities::BoldOff << std::endl;
}
