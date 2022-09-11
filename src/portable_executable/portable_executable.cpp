#include "portable_executable.h"

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

  // DOS header
  printf("dosHeader.e_magic: 0x%04X\n", this->m_pDosHeader->e_magic);
  printf("dosHeader.e_lfanew: 0x%08X\n", this->m_pDosHeader->e_lfanew);
  printf("dosHeader numPages: %i\n", this->m_pDosHeader->e_cp);

  // PE header
  printf("peHeader.Magic 0x%08X\n", this->m_pPeHeader->Magic);
  printf("peHeader.Machine 0x%04X\n", this->m_pPeHeader->Machine);
  printf("peHeader.NumberOfSections %i\n", this->m_pPeHeader->NumberOfSections);
  printf("peHeader.NumberOfSymbols %i\n", this->m_pPeHeader->NumberOfSymbols);
  printf("peHeader.SizeOfOptionalHeader %i\n",
         this->m_pPeHeader->SizeOfOptionalHeader);

  // PE optional header
  printf("opHeader image ver %i.%i\n", this->m_pOpHeader->MajorImageVersion,
         this->m_pOpHeader->MinorImageVersion);
  printf("opHeader OS ver %i.%i\n",
         this->m_pOpHeader->MajorOperatingSystemVersion,
         this->m_pOpHeader->MinorOperatingSystemVersion);
  printf("opHeader linker ver %i.%i\n", this->m_pOpHeader->MajorLinkerVersion,
         this->m_pOpHeader->MinorLinkerVersion);
}

void PortableExecutable::PrintSectionInfo() {
  // PE section headers
  for (int i = 0; i < this->m_pPeHeader->NumberOfSections; i++) {
    auto pSectionHeader = this->m_pSectionHeaders[i];

    printf("section %i name %s\n", i, pSectionHeader.Name);

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
  }
}
