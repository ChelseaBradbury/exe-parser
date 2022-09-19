#pragma once

#include "dos_header.h"
#include "pe_header.h"

#include "memory_map.h"

#include "../cpputils/struct_pack.h"

#include <string>

class PortableExecutable {
public:
  PortableExecutable(uint8_t* pFile, uint32_t fileSizeBytes);
  ~PortableExecutable();

  void PrintFileInfo();
  void PrintSectionInfo();

protected:
  void MapHeaders();
  void MapMemory();

  uint8_t* m_pFile = nullptr;
  uint32_t m_fileSizeBytes = 0;

  DosHeader* m_pDosHeader = nullptr;
  PeHeader* m_pPeHeader = nullptr;
  PeOptionalHeader* m_pOpHeader = nullptr;
  PeSectionHeader* m_pSectionHeaders = nullptr;

  MemoryMap m_memoryMap;
};
