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

PACK(struct ResourceDirectoryNameEntry {
  uint32_t NameOffset;
  uint32_t Offset;
});

PACK(struct ResourceDirectoryIdEntry {
  uint32_t IntegerId;
  uint32_t Offset;
});

const uint32_t EntryOffsetMask = 0b10000000000000000000000000000000;

uint32_t GetDataEntryOffset(uint32_t offset);
uint32_t GetSubdirectoryOffset(uint32_t offset);
