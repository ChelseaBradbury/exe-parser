#include "portable_executable.h"

#include <string>

#include "../cpputils/print.h"

PortableExecutable::PortableExecutable(uint8_t* pFile, uint32_t fileSizeBytes) {
  this->m_pFile = pFile;
  this->m_fileSizeBytes = fileSizeBytes;

  this->MapHeaders();
  this->MapMemory();

  this->MapImportDirectories();

  this->ParseResourceSection();
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

void PortableExecutable::MapMemory() {
  for (int i = 0; i < this->m_pPeHeader->NumberOfSections; i++) {
    auto secHdr = this->m_pSectionHeaders[i];

    this->m_memoryMap.MapRange(secHdr.VirtualAddress, secHdr.Misc.VirtualSize,
                               secHdr.PointerToRawData, secHdr.SizeOfRawData);
  }

  this->m_memoryMap.PrintFileRanges();

  for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++) {
    auto dir = this->m_pOpHeader->DataDirectory[i];

    if (dir.Size == 0) {
      continue;
    }

    uint32_t fileOffset = this->m_memoryMap.GetFileOffset(dir.VirtualAddress);
    printf("dir %i virt: 0x%08X file: 0x%08X size: %i\n", i, dir.VirtualAddress,
           fileOffset, dir.Size);

    //PrintHex(this->m_pFile + fileOffset, dir.Size, 48);
  }
  printf("\n");
}

void PortableExecutable::MapImportDirectories() {
  // Import Table
  auto dir = this->m_pOpHeader->DataDirectory[1];

  if (dir.Size == 0) {
    // No imports
    this->m_numImportDirectories = 0;
    return;
  }

  uint32_t fileOffset = this->m_memoryMap.GetFileOffset(dir.VirtualAddress);
  this->m_pImportDirectories = (ImportDirectory*)(this->m_pFile + fileOffset);

  // Count import directories
  uint32_t i = 0;
  for (; i < dir.Size / sizeof(ImportDirectory); i++) {
    auto imp = this->m_pImportDirectories[i];
    if (imp.ImportLookupTableRVA == 0) {
      // This is the entry after the last valid entry
      break;
    }

    uint32_t nameFileOffset = this->m_memoryMap.GetFileOffset(imp.NameRVA);
    printf("import %s\n", (char*)(this->m_pFile + nameFileOffset));
  }
  printf("\n");
}

void PortableExecutable::RecurseResourceTree(
    ResourceDirectoryHeader* pRoot, ResourceDirectoryHeader* pResourceHeader,
    uint32_t depth) {

  char indent[255];
  sprintf(indent, "%%%is", depth * 4);

  printf(indent, "");
  printf("*ResourceDirectoryHeader @ %p:\n", (void*)pResourceHeader);
  printf(indent, "");
  printf("v%i.%i nameEntries: %i idEntries: %i\n",
         pResourceHeader->MajorVersion, pResourceHeader->NumberOfNameEntries,
         pResourceHeader->NumberOfIdEntries, pResourceHeader->MinorVersion);
  printf("\n");

  auto pNameEntries =
      (ResourceDirectoryNameEntry*)((uint8_t*)(pResourceHeader) +
                                    sizeof(ResourceDirectoryHeader));

  auto pIdEntries =
      (ResourceDirectoryIdEntry*)&pNameEntries[pResourceHeader
                                                   ->NumberOfNameEntries];

  for (uint32_t i = 0; i < pResourceHeader->NumberOfNameEntries; i++) {
    auto entry = pNameEntries[i];

    printf(indent, "");
    printf("*NAME Entry %i nameOffset: 0x%08X\n", i, entry.NameOffset);

    auto dataEntryOffset = GetDataEntryOffset(entry.Offset);
    if (dataEntryOffset > 0) {
      printf(indent, "");
      printf("dataEntryOffset 0x%08X\n", dataEntryOffset);
    }
    auto subdirectoryOffset = GetSubdirectoryOffset(entry.Offset);
    if (subdirectoryOffset > 0) {
      printf(indent, "");
      printf("subdirectoryOffset 0x%08X\n", subdirectoryOffset);

      printf("\n");
      RecurseResourceTree(
          pRoot,
          (ResourceDirectoryHeader*)((uint8_t*)(pRoot) + subdirectoryOffset),
          depth + 1);
    }
  }

  for (uint32_t i = 0; i < pResourceHeader->NumberOfIdEntries; i++) {
    auto entry = pIdEntries[i];
    printf(indent, "");
    printf("*ID Entry %i id: 0x%08X\n", i, entry.IntegerId);
    auto dataEntryOffset = GetDataEntryOffset(entry.Offset);
    if (dataEntryOffset > 0) {
      printf(indent, "");
      printf("dataEntryOffset 0x%08X\n", dataEntryOffset);
      auto pDataEntry = (ResourceDataEntry*)((uint8_t*)pRoot + dataEntryOffset);

      printf("\n");
      printf(indent, "");
      printf("*DataEntry offset: 0x%08X size: %i codePage: 0x%08X\n",
             pDataEntry->DataRVA, pDataEntry->Size, pDataEntry->Codepage);

      auto pData =
          this->m_pFile + this->m_memoryMap.GetFileOffset(pDataEntry->DataRVA);
      PrintHex(pData, pDataEntry->Size, 96);
    }
    auto subdirectoryOffset = GetSubdirectoryOffset(entry.Offset);
    if (subdirectoryOffset > 0) {
      printf(indent, "");
      printf("subdirectoryOffset 0x%08X\n", subdirectoryOffset);

      printf("\n");
      RecurseResourceTree(
          pRoot,
          (ResourceDirectoryHeader*)((uint8_t*)(pRoot) + subdirectoryOffset),
          depth + 1);
    }
    printf("\n");
  }
}

void PortableExecutable::ParseResourceSection() {
  // Resource Directory
  auto dir = this->m_pOpHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];

  if (dir.Size == 0) {
    // No resources
    return;
  }

  uint32_t fileOffset = this->m_memoryMap.GetFileOffset(dir.VirtualAddress);

  auto pResourceHeader = (ResourceDirectoryHeader*)(this->m_pFile + fileOffset);

  PrintHex(this->m_pFile + fileOffset, 64, 48);
  printf("\n");

  RecurseResourceTree(pResourceHeader, pResourceHeader, 0);

  printf("\n");
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
  printf("opHeader BaseOfCode 0x%08X\n", this->m_pOpHeader->BaseOfCode);
  printf("opHeader BaseOfData 0x%08X\n", this->m_pOpHeader->BaseOfData);
  printf("opHeader ImageBase 0x%08X\n", this->m_pOpHeader->ImageBase);
  printf("\n");
}

void PortableExecutable::PrintSectionInfo() {
  for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++) {
    auto dir = this->m_pOpHeader->DataDirectory[i];

    uint32_t dirOffset = (uint8_t*)(&this->m_pOpHeader->DataDirectory[i]) -
                         (uint8_t*)this->m_pOpHeader;

    printf("directory %i(%i/0x%08X): 0x%08X, %i\n", i, dirOffset, dirOffset,
           dir.VirtualAddress, dir.Size);
  }
  printf("\n");

  // PE section headers
  for (int i = 0; i < this->m_pPeHeader->NumberOfSections; i++) {
    auto pSectionHeader = &this->m_pSectionHeaders[i];

    uint32_t offset =
        reinterpret_cast<uint8_t*>(&this->m_pSectionHeaders[i]) - this->m_pFile;

    printf("section %i %s 0x%08X -> %p\n", i, pSectionHeader->Name, offset,
           pSectionHeader);

    printf("  VirtualAddress 0x%08X\n", pSectionHeader->VirtualAddress);
    printf("  SizeOfRawData 0x%08X\n", pSectionHeader->SizeOfRawData);
    printf("  PointerToRawData 0x%08X\n", pSectionHeader->PointerToRawData);
    printf("  PointerToRelocations 0x%08X\n",
           pSectionHeader->PointerToRelocations);
    printf("  PointerToLinenumbers 0x%08X\n",
           pSectionHeader->PointerToLinenumbers);
    printf("  NumberOfRelocations 0x%08X\n",
           pSectionHeader->NumberOfRelocations);
    printf("  NumberOfLinenumbers 0x%08X\n",
           pSectionHeader->NumberOfLinenumbers);
    printf("  Characteristics 0x%08X\n", pSectionHeader->Characteristics);
    printf("\n");

    /*
    PrintHex(this->m_pFile + pSectionHeader.PointerToRawData,
             pSectionHeader.SizeOfRawData, 48);
    */

    printf("\n");
  }
}
