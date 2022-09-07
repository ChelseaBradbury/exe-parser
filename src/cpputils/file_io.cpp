#include "file_io.h"

int loadFileIntoMemory(std::string filePath, char *&pBuffer) {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);
  int length = file.tellg();
  file.seekg(0, std::ios::beg);

  pBuffer = new char[length + 1];
  pBuffer[length] = 0x00;

  if (file.read(pBuffer, length)) {
    file.close();
    return length;
  }
  file.close();
  delete[] pBuffer;
  pBuffer = NULL;
  return 0;
}
