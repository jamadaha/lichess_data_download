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

    static void AddFileDownloads(Downloader &downloader, DownloadList &downloads, std::string downloadPath) {
        for (auto download : downloads)
            downloader.AddDownload(download.link, 
                                   downloadPath + std::to_string(download.year) + '/' + std::to_string(download.month), 
                                   DownloadType::Binary);
    }

    static void DownloadData(std::string tempPath, std::string downloadPath, std::string range) {
        std::cout << Utilities::BoldOn << "----BEGINNING DOWNLOAD----" << Utilities::BoldOff << std::endl;
        Downloader downloader = Downloader();
        downloader.Init();

        DownloadList availableFiles = DownloadFileAvailablity(downloader, tempPath, range);
        AddFileDownloads(downloader, availableFiles, downloadPath);
   
        auto bar = Utilities::GenerateProgressBar();
        while (downloader.LoadNextDownload()) {
            std::cout << Utilities::BoldOn << "Downloading " << Utilities::BoldOff << downloader.GetDownloadLink() << std::endl;
            while (downloader.Update()) {
                double progress = downloader.GetDownloadProgress();
                bar.set_progress(downloader.GetDownloadProgress() * 100);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            bar.set_progress(100);
        }
    
        downloader.Clean();
        std::cout << Utilities::BoldOn << "----FINISHED  DOWNLOAD----" << Utilities::BoldOff << std::endl;
    }
}

