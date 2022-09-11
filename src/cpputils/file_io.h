#pragma once

#include <fstream>
#include <sys/stat.h>

char* loadFileIntoMemory(std::string filePath, int& length);
