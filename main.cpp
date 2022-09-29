#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <stdlib.h>

#include "include/indicators/indicators.hpp"
#include "src/Downloader.hpp"
#include "src/DownloadParser.hpp"
#include "src/Utilities.hpp"
#include "src/ArgumentHandler.hpp"

int main(int argc, char** argv) {
    ArgumentHandler aH;
    int isHelp = aH.GetOptions(argc, argv);
    if (isHelp) std::exit(0);

    std::cout << Utilities::BoldOn << "----BEGINNING DOWNLOAD----" << Utilities::BoldOff << std::endl;
    Downloader::Init();
    
    std::vector<Download> allDownloads = DownloadParser::GetDownloads(aH.tempPath);
    std::vector<Download> rangedDownloads = DownloadParser::GetRange(allDownloads, aH.range);
    Downloader downloader = Downloader();
    for (int i = rangedDownloads.size() - 1; i >= 0; i--) {
        Download download = rangedDownloads[i];
        downloader.AddDownload(download.link, 
        aH.downloadPath + std::to_string(download.year) + "-" + std::to_string(download.month) + ".pgn.bz2", 
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
    Downloader::Clean();
    std::cout << Utilities::BoldOn << "----FINISHED  DOWNLOAD----" << Utilities::BoldOff << std::endl;
    
    return 0;
}
