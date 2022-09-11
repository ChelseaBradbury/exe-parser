#include "file_io.h"

char* loadFileIntoMemory(std::string filePath, int& length) {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);
  length = file.tellg();
  file.seekg(0, std::ios::beg);

  char* pFile = new char[length + 1];
  pFile[length] = 0x00;

  file.read(pFile, length);
  file.close();
  return pFile;
}
