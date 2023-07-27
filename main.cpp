#include <thread>
#include <string>
#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <filesystem>
#include "headers/threadPool.h"
#include "headers/checkFiles.h"

using namespace std;

const int MAX_THREADS = 8;
const string SEARCH_DIR_PATH = "C:\\";
extern const string SEARCHED_FILE = "unknown_demon";

int main()
{
    // Check if our file is not in root folder
    for (const auto &object : filesystem::directory_iterator(SEARCH_DIR_PATH))
    {
        if (!filesystem::is_directory(object) && removeExtension(object) == SEARCHED_FILE)
        {
            cout << object.path() << endl;
            return 0;
        }
    }

    int counter = 0;
    ThreadPool threadPool(MAX_THREADS);

    // If not then going through all C-subdirectories and creating for each individual thread
    for (const auto &object : filesystem::directory_iterator(SEARCH_DIR_PATH))
    {
        if (filesystem::is_directory(object))
        {
            threadPool.enqueue(checkFiles, counter, object.path().string());
            counter++;
        }
    }
}