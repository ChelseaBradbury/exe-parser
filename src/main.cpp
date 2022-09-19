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
  std::string filepath = "./exe/BF2.exe";

  int length;
  char* pFile = loadFileIntoMemory(filepath, length);

  printf("PE file length: %i\n", length);

  PortableExecutable pe((uint8_t*)pFile, int32_t(length));

  pe.PrintFileInfo();
  pe.PrintSectionInfo();

  return 0;
}
