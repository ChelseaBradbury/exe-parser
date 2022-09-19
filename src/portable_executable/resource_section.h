#pragma once

#include <stdint.h>

#include "../cpputils/struct_pack.h"

PACK(struct ResourceDirectoryHeader {
  uint32_t Characteristics;
  uint32_t Timestamp;
  uint16_t MajorVersion;
  uint16_t MinorVersion;
  uint16_t NumberOfNameEntries;
  uint16_t NumberOfIdEntries;
});

PACK(struct ResourceDirectoryEntry {
  uint32_t NameOffset;
  uint32_t IntegerId;
  uint32_t DataEntryOffset;
  uint32_t SubdirectoryOffset;
});
