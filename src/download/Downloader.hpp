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
#include <vector>

#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3000000

enum class DownloadType {
    Text,
    Binary
};

struct FileDownload {
public:
    std::string url;
    std::string path;
    FILE *bFile = nullptr;
    std::ofstream *tFile = nullptr;
    double *progress;
    DownloadType downloadType;
    FileDownload(std::string url, std::string path, DownloadType type) : url(url), path(path), downloadType(type) {
        progress = new double;
    }
    ~FileDownload() {
        if (bFile)
            fclose(bFile);
        if (tFile) {
            tFile->close();
            delete(tFile);
        }
        delete(progress);
    }
};

class Downloader {
public:
    Downloader();
    ~Downloader();
    static void Init();
    static void Clean();
    void AddDownload(std::string url, std::string path, DownloadType downloadType);
    bool LoadNextDownload();
    double GetDownloadProgress();
    std::string GetDownloadLink();
    std::string GetDownloadPath();
    bool Update();

private:
    std::vector<FileDownload*> downloads;
    std::vector<FileDownload*> finishedDownloads;
    CURLM* multiHandle;
    static size_t TWrite(void *ptr, size_t size, size_t nmemb, void *stream);
    static size_t BWrite(void *ptr, size_t size, size_t nmemb, void *stream);
    static int xferinfo(void *ptr, double dltotal, double dlnow, double ultotal, double ulnow);
    
    void AddDir(std::string path);
};

#endif
