#include "headers/checkFiles.h"
#include <string>
#include <filesystem>
#include <mutex>
#include <atomic>
#include <iostream>

using namespace std;

extern const string SEARCHED_FILE;
atomic<bool> stopAllThreads(false);
mutex stopMutex;

string removeExtension(const filesystem::directory_entry &object)
{
    string fileName = object.path().filename().string();
    size_t lastDot = fileName.find_last_of(".");
    if (lastDot != string::npos)
    {
        return fileName.substr(0, lastDot);
    }
    return fileName;
}

void checkFiles(int threadId, const string &current_path)
{
    try
    {
        for (const auto &object : filesystem::recursive_directory_iterator(current_path, filesystem::directory_options::skip_permission_denied))
        {
            stopMutex.lock();

            if (stopAllThreads)
            {
                stopMutex.unlock();
                return;
            }

            stopMutex.unlock();
            if (removeExtension(object) == SEARCHED_FILE)
            {
                cout << object.path() << endl;

                stopMutex.lock();
                stopAllThreads = true;
                stopMutex.unlock();
            }
        }
    }
    catch (const filesystem::filesystem_error &ex)
    {
        // skip if we dont have access to folder
    }
}
