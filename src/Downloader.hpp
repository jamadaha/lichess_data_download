#ifndef DOWNLOADER
#define DOWNLOADER

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <curl/curl.h>
#include <filesystem>
#include <unordered_set>
#include <algorithm>

#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3000000

struct Progress {
    double progressPercent;
    Progress(){
        progressPercent = 0;
    }
};

struct Download {
public:
    std::string url;
    std::string path;
    FILE *file;
    Progress *progress;
    Download(){};
    Download(std::string url, std::string path) : url(url), path(path){
        progress = new Progress();
    }
};

class Downloader {
public:
    Downloader();
    ~Downloader();
    static void Init();
    static void Clean();
    void AddDownload(std::string url, std::string path);
    bool LoadNextDownload();
    double GetDownloadProgress();
    bool Update();

private:
    std::vector<Download> downloads;
    CURLM* multiHandle;
    static size_t Write(void *ptr, size_t size, size_t nmemb, void *stream);
    static int xferinfo(void *ptr, double dltotal, double dlnow, double ultotal, double ulnow);
    
    void AddDir(std::string path);
};

#endif