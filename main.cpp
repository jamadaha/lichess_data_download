#include <iostream>
#include <string>
#include <unistd.h>
#include "include/cxxopts/cxxopts.hpp"
#include "include/indicators/indicators.hpp"
#include "src/Downloader.hpp"


int main(int argc, char** argv) {
    cxxopts::Options options("LichessToDB", "Downloads Lichess data, extracts it, then inserts into local Postgres instance");

    options.add_options()
        ("h,help", "Print usage")
        ("p,path", "Temporary path for download and extraction", cxxopts::value<std::string>()->default_value("./Temp"))
        ("r,range", "Download all months within the spcified range", cxxopts::value<std::string>()->default_value("01/2013-12/2021"))
        ("e,extract", "Extract downloaded files", cxxopts::value<int>()->default_value("1"))
    ;

    auto result = options.parse(argc, argv);

    Downloader::Init();
    Downloader downloader = Downloader();
    downloader.AddDownload("https://database.lichess.org/standard/list.txt", result["p"].as<std::string>() + "/download_list.txt");
    downloader.AddDownload("https://database.lichess.org/standard/lichess_db_standard_rated_2015-06.pgn.bz2", result["p"].as<std::string>() + "/2015-06.txt");
    
    using namespace indicators;

    while (downloader.LoadNextDownload()) {
        printf("Beginning Download...\n");
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
            sleep(1);
        }
        bar.set_progress(100);
        printf("Finished Download\n");
    }
    Downloader::Clean();
    return 0;
}
