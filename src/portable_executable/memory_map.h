#pragma once

#include <vector>
#include <map>

struct MemoryRange {
  uint32_t start;
  uint32_t end;
  uint32_t mappedToStart;
};

class MemoryMap {
public:
  MemoryMap();
  ~MemoryMap();

  void MapRange(uint32_t virtOffset, uint32_t virtSize, uint32_t fileOffset,
                uint32_t fileSize);

  uint32_t GetVirtualOffset(uint32_t fileOffset);
  uint32_t GetFileOffset(uint32_t virtOffset);

  void PrintFileRanges();
  void PrintVirtualRanges();

protected:
  void PrintRanges(std::map<uint32_t, MemoryRange> ranges);

  std::map<uint32_t, MemoryRange> m_fileRangeByStart;
  std::map<uint32_t, MemoryRange> m_virtRangeByStart;
};
