#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "include/cxxopts/cxxopts.hpp"
#include "include/indicators/indicators.hpp"
#include "src/Downloader.hpp"
#include "src/DownloadParser.hpp"
#include "src/Utilities.hpp"

cxxopts::ParseResult GetOptions(int argc, char** argv) {
    cxxopts::Options options("LichessToDB", "Downloads Lichess data, extracts it, then inserts into local Postgres instance");

    options.add_options()
        ("h,help", "Print usage")
        ("p,path", "Temporary path for download and extraction", cxxopts::value<std::string>()->default_value("./Temp"))
        ("r,range", "Download all months within the spcified range", cxxopts::value<std::string>()->default_value("01/2013-01/2014"))
        ("e,extract", "Extract downloaded files", cxxopts::value<int>()->default_value("1"))
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl; exit(0);
    }
    return result;
}

int main(int argc, char** argv) {
    auto result = GetOptions(argc, argv);

    std::cout << Utilities::BoldOn << "----BEGINNING DOWNLOAD----" << Utilities::BoldOff << std::endl;
    Downloader::Init();
    
    std::vector<Download> allDownloads = DownloadParser::GetDownloads(result["p"].as<std::string>());
    std::vector<Download> rangedDownloads = DownloadParser::GetRange(allDownloads, result["r"].as<std::string>());
    Downloader downloader = Downloader();
    for (int i = rangedDownloads.size() - 1; i >= 0; i--) {
        Download download = rangedDownloads[i];
        downloader.AddDownload(download.link, 
        result["p"].as<std::string>() + "/Downloads/" + std::to_string(download.year) + "-" + std::to_string(download.month) + ".pgn.bz2", 
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
    std::cout << Utilities::BoldOn << "----FINISHED DOWNLOAD----" << Utilities::BoldOff << std::endl;
    //system("");
    return 0;
}
