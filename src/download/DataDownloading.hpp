#include <string>
#include <thread>
#include "indicators/indicators.hpp"
#include "Utilities.hpp"
#include "types/Download.hpp"
#include "types/DownloadList.hpp"
#include "Downloader.hpp"
#include "DownloadParsing.hpp"

namespace DataDownloading { 
    static DownloadList DownloadFileAvailablity(Downloader &downloader, std::string tempPath, std::string range) {
        const std::string listPath = tempPath + "/download_list.txt";

        downloader.AddDownload(DownloadParsing::AVAILABLE_DOWNLOAD_LINK, listPath, DownloadType::Text);
        downloader.LoadNextDownload();
        while(downloader.Update()) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
        
        return DownloadParsing::ParseDownloadFile(listPath).GetRange(range);
    }

    // Removes those from downloads which are already present at downloadPath
    static void FilterDownloaded(std::string downloadPath, DownloadList &downloads) {
        const auto path(downloadPath);
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            const auto entryPath = entry.path().string();
            const auto fileName = entryPath.substr(entryPath.find_last_of("/\\") + 1);
            if (fileName.find(".temp") != std::string::npos)
                continue;
            downloads.RemoveDownload(fileName);
        }
    }

    static void AddFileDownloads(Downloader &downloader, DownloadList &downloads, std::string downloadPath) {
        for (auto download : downloads)
            downloader.AddDownload(download.link, 
                                   downloadPath + download.fileName + ".temp", 
                                   DownloadType::Binary);
    }

    static void DownloadData(std::string tempPath, std::string downloadPath, std::string range) {
        std::cout << Utilities::BoldOn << "----BEGINNING DOWNLOAD----" << Utilities::BoldOff << std::endl;
        Downloader downloader = Downloader();
        downloader.Init();

        DownloadList availableFiles = DownloadFileAvailablity(downloader, tempPath, range);
        FilterDownloaded(downloadPath, availableFiles);
        AddFileDownloads(downloader, availableFiles, downloadPath);
   
        while (downloader.LoadNextDownload()) {
            auto filePath = downloader.GetDownloadPath();
            std::cout << Utilities::BoldOn << "Downloading " << Utilities::BoldOff << downloader.GetDownloadLink() << std::endl;
            auto bar = Utilities::GenerateProgressBar();
            while (downloader.Update()) {
                double progress = downloader.GetDownloadProgress();
                bar.set_progress(downloader.GetDownloadProgress() * 100);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            // Remove .temp extension
            auto newPath = filePath.substr(0, filePath.size() - 5);
            std::rename(filePath.c_str(), newPath.c_str());
            bar.set_progress(100);
        }
    
        downloader.Clean();
        std::cout << Utilities::BoldOn << "----FINISHED  DOWNLOAD----" << Utilities::BoldOff << std::endl;
    }
}

