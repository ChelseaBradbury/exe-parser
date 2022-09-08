#include <string>

#include "cpputils/file_io.h"

#include "dos_header.h"
#include "pe_header.h"
#include "pe_section.h"

void printHex(uint8_t *pData, uint32_t numBytes)
{
    for (uint32_t i = 0; i < numBytes; i++)
    {
        uint32_t temp = *(pData + i);
        printf(" 0x%02X", temp);
    }
    printf("\n");
}

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
    printf("dosHeader numPages: %i\n", dosHeader.e_cp);

    uint32_t peMagic;
    memcpy(&peMagic, pFile + dosHeader.e_lfanew, sizeof(peMagic));

    PeHeader peHeader;
    memcpy(&peHeader, pFile + dosHeader.e_lfanew + sizeof(peMagic), sizeof(peHeader));

    // printHex((uint8_t *)(pFile + dosHeader.e_lfanew), 16);

    printf("pe machine 0x%04X\n", peHeader.Machine);
    printf("pe sections %i\n", peHeader.NumberOfSections);
    printf("pe symbols %i\n", peHeader.NumberOfSymbols);
    printf("pe optional size %i\n", peHeader.SizeOfOptionalHeader);

    PeOptionalHeader peOptional;
    memcpy(&peOptional, pFile + dosHeader.e_lfanew + sizeof(peMagic) + sizeof(peHeader), sizeof(peOptional));

    printf("image major %i minor %i \n", peOptional.MajorImageVersion, peOptional.MinorImageVersion);
    printf("OS major %i minor %i \n", peOptional.MajorOperatingSystemVersion, peOptional.MinorOperatingSystemVersion);
    printf("linker major %i minor %i \n", peOptional.MajorLinkerVersion, peOptional.MinorLinkerVersion);

    uint32_t sectionOffset = dosHeader.e_lfanew + sizeof(peMagic) + sizeof(peHeader) + sizeof(peOptional);

    // .text = code
    // .rdata = read-only data
    // .data = data
    // .idata = initialized data
    // .rsrc = resource
    // .reloc = relocate

    PeSectionHeader peSection;
    for (int i = 0; i < peHeader.NumberOfSections; i++)
    {
        memcpy(&peSection, pFile + sectionOffset + (sizeof(peSection) * i), sizeof(peSection));

        printf("section %i name %s\n", i, peSection.Name);
    }

    return 0;
}
