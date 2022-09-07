#include <string>

#include "cpputils/file_io.h"

int main()
{
    std::string filepath = "./exe/BF2_r.exe";

    int length;
    char *pFile = loadFileIntoMemory(filepath, length);

    printf("length: %i\n", length);

    return 0;
}