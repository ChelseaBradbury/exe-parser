#pragma once

#include <fstream>
#include <sys/stat.h>

int loadFileIntoMemory(std::string filePath, char *&pBuffer);
