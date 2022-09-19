#include "memory_map.h"

#include <algorithm>

MemoryMap::MemoryMap() {}

MemoryMap::~MemoryMap() {}

void MemoryMap::MapRange(uint32_t virtOffset, uint32_t virtSize,
                         uint32_t fileOffset, uint32_t fileSize) {
  MemoryRange virtRange;
  virtRange.start = virtOffset;
  virtRange.end = virtOffset + virtSize;
  virtRange.mappedToStart = fileOffset;

  this->m_virtRangeByStart[virtOffset] = virtRange;

  MemoryRange fileRange;
  fileRange.start = fileOffset;
  fileRange.end = fileOffset + fileSize;
  fileRange.mappedToStart = virtOffset;

  this->m_fileRangeByStart[fileOffset] = fileRange;
}

uint32_t MemoryMap::GetVirtualOffset(uint32_t fileOffset) {
  MemoryRange fileRange;
  for (auto const& [start, range] : this->m_fileRangeByStart) {
    if (start > fileOffset) {
      break;
    }
    fileRange = range;
  }

  uint32_t fileRangeOffset = fileOffset - fileRange.start;

  MemoryRange virtRange = this->m_virtRangeByStart[fileRange.mappedToStart];

  return virtRange.start + fileRangeOffset;
}

uint32_t MemoryMap::GetFileOffset(uint32_t virtOffset) {
  MemoryRange virtRange;
  for (auto const& [start, range] : this->m_virtRangeByStart) {
    if (start > virtOffset) {
      break;
    }
    virtRange = range;
  }

  uint32_t virtRangeOffset = virtOffset - virtRange.start;

  MemoryRange fileRange = this->m_fileRangeByStart[virtRange.mappedToStart];

  return fileRange.start + virtRangeOffset;
}

void MemoryMap::PrintFileRanges() {
  printf("file ranges:\n");
  this->PrintRanges(this->m_fileRangeByStart);
  printf("\n");
}

void MemoryMap::PrintVirtualRanges() {
  printf("virtual ranges:\n");
  this->PrintRanges(this->m_virtRangeByStart);
  printf("\n");
}

void MemoryMap::PrintRanges(std::map<uint32_t, MemoryRange> ranges) {
  for (auto const& [start, range] : ranges) {
    printf("start 0x%08X mappedTo 0x%08X\n", range.start, range.mappedToStart);
  }
}