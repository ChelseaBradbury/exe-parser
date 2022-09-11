#pragma once

#include "dos_header.h"
#include "pe_header.h"

#include "../cpputils/struct_pack.h"

#include <string>

class PortableExecutable {
public:
  PortableExecutable(uint8_t* pFile);
  ~PortableExecutable();

  void PrintFileInfo();
  void PrintSectionInfo();

protected:
  void MapHeaders();

  uint8_t* m_pFile = nullptr;

  DosHeader* m_pDosHeader = nullptr;
  PeHeader* m_pPeHeader = nullptr;
  PeOptionalHeader* m_pOpHeader = nullptr;
  PeSectionHeader* m_pSectionHeaders = nullptr;
};
