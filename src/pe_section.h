#pragma once

#include <stdint.h>

#include "cpputils/struct_pack.h"

#define IMAGE_SIZEOF_SHORT_NAME 0x08

PACK(struct PeSectionHeader {
    uint8_t Name[IMAGE_SIZEOF_SHORT_NAME];
    union
    {
        uint32_t PhysicalAddress;
        uint32_t VirtualSize;
    } Misc;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
});
