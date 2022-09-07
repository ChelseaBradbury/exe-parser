#include <string>

#include "cpputils/file_io.h"

#include "dos_header.h"

int main()
{
    std::string filepath = "./exe/BF2_r.exe";

    int length;
    char *pFile = loadFileIntoMemory(filepath, length);

    printf("length: %i\n", length);

    DosHeader dosHeader;
    memcpy(&dosHeader, pFile, sizeof(dosHeader));

    printf("dosHeader.e_magic: 0x%04X\n", dosHeader.e_magic);

    return 0;
}
