#include "Downloader.hpp"

Downloader::Downloader() {
    multiHandle = curl_multi_init();
    curl_multi_setopt(multiHandle, CURLMOPT_MAXCONNECTS, (long)1);
}

Downloader::~Downloader() {
    curl_multi_cleanup(multiHandle);
}

void Downloader::Init() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void Downloader::Clean() {
    curl_global_cleanup();
}

void Downloader::AddDownload(std::string url, std::string path) {
    AddDir(path);
    downloads.push_back(Download(url, path));
    LoadNextDownload();
}

double Downloader::GetDownloadProgress() {
    return downloads[0].progress->progressPercent;
}

void Downloader::LoadNextDownload() {
    if (downloads.size() == 0)
        return;
    Download download = downloads[0];

    FILE* file = fopen(download.path.c_str(), "wb");
    download.file = file;
    CURL *transfer = curl_easy_init();
    curl_easy_setopt(transfer, CURLOPT_WRITEFUNCTION, Write);
    curl_easy_setopt(transfer, CURLOPT_URL, download.url.c_str());
    curl_easy_setopt(transfer, CURLOPT_WRITEDATA, file);
    Progress* p = download.progress;
    curl_easy_setopt(transfer, CURLOPT_PROGRESSDATA, p);
    curl_easy_setopt(transfer, CURLOPT_PROGRESSFUNCTION, xferinfo);
    curl_easy_setopt(transfer, CURLOPT_NOPROGRESS, false);
    curl_multi_add_handle(multiHandle, transfer);
}

bool Downloader::Update() {
    int runningCount = 0;
    curl_multi_perform(multiHandle, &runningCount);
    if (runningCount == 0)
        return 0;
    CURLMsg *msg;
    int msgs_left = -1;
    while((msg = curl_multi_info_read(multiHandle, &msgs_left))) {
        if(msg->msg == CURLMSG_DONE) {
            printf("qwe\n");
            CURL *e = msg->easy_handle;
            curl_multi_remove_handle(multiHandle, e);
            curl_easy_cleanup(e);
            downloads.erase(downloads.begin());
            return 0;
        }
        else {
            fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
        }
    }
    return 1;
}

size_t Downloader::Write(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

int Downloader::xferinfo(void *p, double dltotal, double dlnow, double ultotal, double ulnow) {
    struct Progress *progress = (struct Progress*) p;

    if (dltotal > 0.1 && dlnow > 0.1) {
        double newProgress = dlnow / dltotal;
        (*progress).progressPercent = newProgress;
    }
    return 0;
}

void Downloader::AddDir(std::string path) {
    std::reverse(path.begin(), path.end());
    int index = path.find("/");
    if (index == path.npos)
        return;
    path = path.substr(index);
    std::reverse(path.begin(), path.end());
    std::filesystem::create_directories(path);
}
