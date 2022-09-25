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
#include <memory>

#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3000000

struct Progress {
    double progressPercent;
    Progress(){
        progressPercent = 0;
    }
};

struct FileDownload {
public:
    std::string url;
    std::string path;
    std::ofstream *file;
    Progress *progress;
    FileDownload(std::string url, std::string path) : url(url), path(path){
        progress = new Progress();
    }
    ~FileDownload() {
        file->close();
        delete(file);
        delete(progress);
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
    std::string GetDownloadLink();
    bool Update();

private:
    std::vector<FileDownload*> downloads;
    CURLM* multiHandle;
    static size_t Write(void *ptr, size_t size, size_t nmemb, void *stream);
    static int xferinfo(void *ptr, double dltotal, double dlnow, double ultotal, double ulnow);
    
    void AddDir(std::string path);
};

#endif