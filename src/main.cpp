#include <string>

#include "cpputils/file_io.h"

#include "dos_header.h"
#include "pe_header.h"

int main()
{
    std::string filepath = "./exe/BF2_r.exe";

    int length;
    char *pFile = loadFileIntoMemory(filepath, length);

    printf("length: %i\n", length);

    DosHeader dosHeader;
    memcpy(&dosHeader, pFile, sizeof(dosHeader));

    printf("dosHeader.e_magic: 0x%04X\n", dosHeader.e_magic);

    printf("dosHeader.e_lfanew: 0x%08X\n", dosHeader.e_lfanew);

    PeHeader peHeader;
    memcpy(&peHeader, pFile + dosHeader.e_lfanew, sizeof(peHeader));

    PeOptionalHeader peOptional;
    memcpy(&peOptional, pFile + dosHeader.e_lfanew + sizeof(peHeader), sizeof(peOptional));

    printf("image major %i minor %i \n", peOptional.MajorImageVersion, peOptional.MinorImageVersion);

    printf("OS major %i minor %i \n", peOptional.MajorOperatingSystemVersion, peOptional.MinorOperatingSystemVersion);

    printf("linker major %i minor %i \n", peOptional.MajorLinkerVersion, peOptional.MinorLinkerVersion);

    return 0;
}
