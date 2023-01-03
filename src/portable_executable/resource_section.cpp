#include "resource_section.h"

uint32_t GetDataEntryOffset(uint32_t offset) {
  bool isSubdirectoryOffset = offset & EntryOffsetMask;
  if (isSubdirectoryOffset) {
    return 0;
  }
  return offset;
}

uint32_t GetSubdirectoryOffset(uint32_t offset) {
  bool isSubdirectoryOffset = offset & EntryOffsetMask;
  if (!isSubdirectoryOffset) {
    return 0;
  }
  return offset ^ EntryOffsetMask;
}
