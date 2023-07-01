#ifndef FILE_UTILITIES
#define FILE_UTILITIES

#include <iostream>
#include <filesystem>
#include <functional>
#include <string>
#include "indicators/indicators.hpp"

namespace Utilities {
    static void DirIterator(std::string path, std::string extension, std::function<void(std::string)> fileCallback) {
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            const auto entryPath = entry.path().string();
            const auto entryExtension = entryPath.substr(entryPath.find_last_of('.') + 1);

            if (entryExtension == extension)
                fileCallback(entryPath);
        }
    }
};

#endif
