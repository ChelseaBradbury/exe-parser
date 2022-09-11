#pragma once

#include "dos_header.h"
#include "pe_header.h"

#include "../cpputils/struct_pack.h"

#include <string>

void getDosHeader(uint8_t *pFile, DosHeader *pDest)
{
    // TODO: check magic number
    memcpy(pDest, pFile, sizeof(DosHeader));
}

uint32_t getPeMagic(uint8_t *pFile, DosHeader *pDosHeader)
{
    return *(uint32_t *)(pFile + pDosHeader->e_lfanew);
}

void getPeHeader(uint8_t *pFile, DosHeader *pDosHeader, PeHeader *pDest)
{
    memcpy(pDest, pFile + pDosHeader->e_lfanew + sizeof(uint32_t), sizeof(PeHeader));
}

void getPeOptionalHeader(uint8_t *pFile, DosHeader *pDosHeader, PeOptionalHeader *pDest)
{
    memcpy(pDest, pFile + pDosHeader->e_lfanew + sizeof(uint32_t) + sizeof(PeHeader), sizeof(PeOptionalHeader));
}

PACK(struct PeHeaderCollection {
    DosHeader dosHeader;
    PeHeader peHeader;
    PeOptionalHeader opHeader;
    PeSectionHeader sectionHeaders[255];
});

/*
PeHeaderCollection getHeaders(uint8_t *pFile)
{
    PeHeaderCollection ret;
    getDosHeader(pFile, &ret.dosHeader);
    ret.peMagic = getPeMagic(pFile, &ret.dosHeader);
    getPeHeader(pFile, &ret.dosHeader, &ret.peHeader);
    getPeOptionalHeader(pFile, &ret.dosHeader, &ret.opHeader);

    ret.pSectionHeaders = new PeSectionHeader[ret.peHeader.NumberOfSections];
    for (int i = 0; i < ret.peHeader.NumberOfSections; i++)
    {
        auto headerOffset = ret.dosHeader.e_lfanew + sizeof(uint32_t) + sizeof(PeHeader) + sizeof(PeOptionalHeader) + (sizeof(PeSectionHeader) * i);

        printf("headerOffset %i\n", headerOffset);
        auto dest = ret.pSectionHeaders + (sizeof(PeSectionHeader) * i);

        memcpy(dest, pFile + headerOffset, sizeof(PeSectionHeader));
    }

    return ret;
}
*/

PeHeaderCollection getHeaders(uint8_t *pFile)
{
    PeHeaderCollection collection;

    // DOS Header
    memcpy(&collection.dosHeader, pFile, sizeof(DosHeader));

    printf("dosHeader.e_magic: 0x%04X\n", collection.dosHeader.e_magic);
    printf("dosHeader.e_lfanew: 0x%08X\n", collection.dosHeader.e_lfanew);
    printf("dosHeader numPages: %i\n", collection.dosHeader.e_cp);

    uint32_t peHeaderOffset = collection.dosHeader.e_lfanew;
    printf("peHeaderOffset: %i\n", peHeaderOffset);

    // PE Header
    memcpy(&collection.peHeader, pFile + peHeaderOffset, sizeof(PeHeader));

    printf("pe machine 0x%04X\n", collection.peHeader.Machine);
    printf("pe sections %i\n", collection.peHeader.NumberOfSections);
    printf("pe symbols %i\n", collection.peHeader.NumberOfSymbols);
    printf("pe optional size %i\n", collection.peHeader.SizeOfOptionalHeader);

    uint32_t opHeaderOffset = peHeaderOffset + sizeof(PeHeader);
    printf("opHeaderOffset: %i\n", opHeaderOffset);

    // PE Optional Header
    memcpy(&collection.opHeader, pFile + opHeaderOffset, sizeof(PeOptionalHeader));

    uint16_t numSections = collection.peHeader.NumberOfSections;
    printf("numSections: %i\n", numSections);

    uint32_t firstSectionHeaderOffset = opHeaderOffset + sizeof(PeOptionalHeader);
    printf("firstSectionHeaderOffset: %i\n", firstSectionHeaderOffset);

    for (uint16_t i = 0; i < numSections; i++)
    {
        uint32_t sectionHeaderOffset = firstSectionHeaderOffset + i * sizeof(PeSectionHeader);
        memcpy(&collection.sectionHeaders[i], pFile + sectionHeaderOffset, sizeof(PeSectionHeader));
    }

    return collection;
}