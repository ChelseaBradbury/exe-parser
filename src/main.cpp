#include <string>

#include "cpputils/file_io.h"

#include "portable_executable/portable_executable.h"

void printHex(uint8_t* pData, uint32_t numBytes) {
  for (uint32_t i = 0; i < numBytes; i++) {
    uint32_t temp = *(pData + i);
    printf(" 0x%02X", temp);
  }
  printf("\n");
}

int main() {
  std::string filepath = "./exe/BF2_r.exe";

  int length;
  char* pFile = loadFileIntoMemory(filepath, length);

  printf("PE file length: %i\n", length);

  auto headerCollection = getHeaders((uint8_t*)pFile);

  printf("got headers\n");

  DosHeader dosHeader = headerCollection.dosHeader;

  printf("dosHeader.e_magic: 0x%04X\n", dosHeader.e_magic);
  printf("dosHeader.e_lfanew: 0x%08X\n", dosHeader.e_lfanew);
  printf("dosHeader numPages: %i\n", dosHeader.e_cp);

  PeHeader peHeader = headerCollection.peHeader;

  printf("pe machine 0x%04X\n", peHeader.Machine);
  printf("pe sections %i\n", peHeader.NumberOfSections);
  printf("pe symbols %i\n", peHeader.NumberOfSymbols);
  printf("pe optional size %i\n", peHeader.SizeOfOptionalHeader);

  PeOptionalHeader peOptional = headerCollection.opHeader;

  printf("image major %i minor %i \n", peOptional.MajorImageVersion,
         peOptional.MinorImageVersion);
  printf("OS major %i minor %i \n", peOptional.MajorOperatingSystemVersion,
         peOptional.MinorOperatingSystemVersion);
  printf("linker major %i minor %i \n", peOptional.MajorLinkerVersion,
         peOptional.MinorLinkerVersion);

  // .text = code
  // .rdata = read-only data
  // .data = data
  // .idata = initialized data
  // .rsrc = resource
  // .reloc = relocate

  for (int i = 0; i < headerCollection.peHeader.NumberOfSections; i++) {
    auto pSectionHeader = headerCollection.sectionHeaders[i];

    printf("section %i name %s\n", i, pSectionHeader.Name);

    printf("  VirtualAddress %p\n", pSectionHeader.VirtualAddress);
    printf("  SizeOfRawData %p\n", pSectionHeader.SizeOfRawData);
    printf("  PointerToRawData %p\n", pSectionHeader.PointerToRawData);
    printf("  PointerToRelocations %p\n", pSectionHeader.PointerToRelocations);
    printf("  PointerToLinenumbers %p\n", pSectionHeader.PointerToLinenumbers);
    printf("  NumberOfRelocations %p\n", pSectionHeader.NumberOfRelocations);
    printf("  NumberOfLinenumbers %p\n", pSectionHeader.NumberOfLinenumbers);
    printf("  Characteristics %p\n", pSectionHeader.Characteristics);
  }

  return 0;
}
