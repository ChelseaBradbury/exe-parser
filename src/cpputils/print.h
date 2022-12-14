#include <string>

void PrintHex(uint8_t* pData, uint32_t dataLen, uint32_t lineLen) {
  uint32_t localLen = dataLen + (lineLen - (dataLen % lineLen));
  uint8_t* pLocal = new uint8_t[localLen];
  printf("dataLen %i localLen %i\n", dataLen, localLen);

  memset(pLocal, 0xFF, localLen);
  memcpy(pLocal, pData, dataLen);

  uint32_t numLines = localLen / lineLen;

  printf("lineLen %i\n", lineLen);
  printf("numLines %i\n", numLines);

  uint8_t* pLine = new uint8_t[lineLen * 5 + lineLen + 1];
  pLine[lineLen] = 0x00;

  for (uint32_t i = 0; i < numLines; i++) {
    //for (int32_t i = numLines - 1; i >= 0; i--) {
    auto lineOffset = lineLen * i;
    //printf("i %i lineOffset %i\n", i, lineOffset);
    for (uint32_t j = 0; j < lineLen; j++) {
      auto charOffset = lineOffset + j;
      printf("%02X", *(pLocal + charOffset));

      if (j % 2 == 1) {
        printf(" ");
      }

      // Copy char to pLine
      uint8_t chr = *(pLocal + charOffset);
      if (chr <= 0x1F || chr > 0x7F) {
        pLine[j] = '.';
      } else {
        pLine[j] = chr;
      }
    }

    printf("%s\n", pLine);
  }

  delete[] pLine;
  delete[] pLocal;
}