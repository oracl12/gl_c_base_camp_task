#ifndef CHECK_FILES_H
#define CHECK_FILES_H

#include <string>
#include <filesystem>

using namespace std;

void checkFiles(int threadId, const string& current_path);
string removeExtension(const filesystem::directory_entry& object);

#endif