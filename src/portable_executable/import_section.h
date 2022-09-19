#pragma once

#include <stdint.h>

#include "../cpputils/struct_pack.h"

PACK(struct ImportDirectory {
  // The RVA of the import lookup table. This table contains a name or ordinal for each
  // import. (The name "Characteristics" is used in Winnt.h, but no longer describes this field.)
  uint32_t ImportLookupTableRVA;
  // The stamp that is set to zero until the image is bound. After the image is bound, this field
  // is set to the time/data stamp of the DLL.
  uint32_t Timestamp;
  // The index of the first forwarder reference.
  uint32_t ForarderChain;
  // The address of an ASCII string that contains the name of the DLL. This address is relative
  // to the image base.
  uint32_t NameRVA;
  // The RVA of the import address table. The contents of this table are identical to the
  // contents of the import lookup table until the image is bound.
  uint32_t ImportAddressTableRVA;
});
