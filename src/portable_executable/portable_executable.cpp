#include "portable_executable.h"

void PrintHex(uint8_t* pData, uint32_t dataLen, uint32_t lineLen) {
  uint32_t localLen = dataLen + (dataLen % lineLen);
  uint8_t* pLocal = new uint8_t[localLen];
  memcpy(pLocal, pData, dataLen);

  uint32_t numLines = localLen / lineLen;

  uint8_t* pLine = new uint8_t[lineLen + 1];
  pLine[lineLen] = 0x00;

  for (uint32_t i = 0; i < numLines; i++) {
    auto lineOffset = lineLen * i;
    for (uint32_t j = 0; j < lineLen; j++) {
      auto offset = lineOffset + j;
      printf("%02X", *(pLocal + offset));

      if (j % 2 == 1) {
        printf(" ");
      }

      // Copy char to pLine
      uint8_t chr = *(pLocal + offset);
      if (chr <= 0x1F) {
        pLine[j] = '.';
      } else {
        pLine[j] = chr;
      }
    }

    printf("%s\n", pLine);
  }

  delete[] pLine;
  delete[] pLocal;
}

PortableExecutable::PortableExecutable(uint8_t* pFile) {
  this->m_pFile = pFile;

  this->MapHeaders();
}

PortableExecutable::~PortableExecutable() {}

void PortableExecutable::MapHeaders() {
  this->m_pDosHeader = reinterpret_cast<DosHeader*>(this->m_pFile);
  this->m_pPeHeader =
      reinterpret_cast<PeHeader*>(this->m_pFile + this->m_pDosHeader->e_lfanew);
  this->m_pOpHeader = reinterpret_cast<PeOptionalHeader*>(
      (uint8_t*)this->m_pPeHeader + sizeof(PeHeader));
  this->m_pSectionHeaders = reinterpret_cast<PeSectionHeader*>(
      (uint8_t*)this->m_pOpHeader + sizeof(PeOptionalHeader));
}

void PortableExecutable::PrintFileInfo() {
  // Headers
  printf("dos %p %lu\n", this->m_pDosHeader, sizeof(DosHeader));
  printf("pe  %p %lu\n", this->m_pPeHeader, sizeof(PeHeader));
  printf("op  %p %lu\n", this->m_pOpHeader, sizeof(PeOptionalHeader));
  printf("sec %p %lu\n", this->m_pSectionHeaders, sizeof(PeSectionHeader));
  printf("\n");

  // DOS header
  printf("dosHeader.e_magic: 0x%04X\n", this->m_pDosHeader->e_magic);
  printf("dosHeader.e_lfanew: 0x%08X\n", this->m_pDosHeader->e_lfanew);
  printf("dosHeader numPages: %i\n", this->m_pDosHeader->e_cp);
  printf("\n");

  // PE header
  printf("peHeader.Magic 0x%08X\n", this->m_pPeHeader->Magic);
  printf("peHeader.Machine 0x%04X\n", this->m_pPeHeader->Machine);
  printf("peHeader.NumberOfSections %i\n", this->m_pPeHeader->NumberOfSections);
  printf("peHeader.NumberOfSymbols %i\n", this->m_pPeHeader->NumberOfSymbols);
  printf("peHeader.SizeOfOptionalHeader %i\n",
         this->m_pPeHeader->SizeOfOptionalHeader);
  printf("\n");

  // PE optional header
  printf("opHeader image ver %i.%i\n", this->m_pOpHeader->MajorImageVersion,
         this->m_pOpHeader->MinorImageVersion);
  printf("opHeader OS ver %i.%i\n",
         this->m_pOpHeader->MajorOperatingSystemVersion,
         this->m_pOpHeader->MinorOperatingSystemVersion);
  printf("opHeader subsys ver %i.%i\n",
         this->m_pOpHeader->MajorSubsystemVersion,
         this->m_pOpHeader->MinorSubsystemVersion);
  printf("opHeader linker ver %i.%i\n", this->m_pOpHeader->MajorLinkerVersion,
         this->m_pOpHeader->MinorLinkerVersion);
  printf("\n");
}

void PortableExecutable::PrintSectionInfo() {
  // PE section headers
  for (int i = 0; i < this->m_pPeHeader->NumberOfSections; i++) {
    auto pSectionHeader = this->m_pSectionHeaders[i];

    printf("section %i %s\n", i, pSectionHeader.Name);

    printf("  VirtualAddress 0x%08X\n", pSectionHeader.VirtualAddress);
    printf("  SizeOfRawData 0x%08X\n", pSectionHeader.SizeOfRawData);
    printf("  PointerToRawData 0x%08X\n", pSectionHeader.PointerToRawData);
    printf("  PointerToRelocations 0x%08X\n",
           pSectionHeader.PointerToRelocations);
    printf("  PointerToLinenumbers 0x%08X\n",
           pSectionHeader.PointerToLinenumbers);
    printf("  NumberOfRelocations 0x%08X\n",
           pSectionHeader.NumberOfRelocations);
    printf("  NumberOfLinenumbers 0x%08X\n",
           pSectionHeader.NumberOfLinenumbers);
    printf("  Characteristics 0x%08X\n", pSectionHeader.Characteristics);
    printf("\n");

    PrintHex(this->m_pFile + pSectionHeader.PointerToRawData,
             pSectionHeader.SizeOfRawData, 48);

    printf("\n");
  }
}
