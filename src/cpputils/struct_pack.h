#pragma once

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK(__Declaration__)                                                  \
  __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

/* Prevents automatic byte alignment (to four bytes, or whatever) of struct data
 * Use like this:
 *
 * PACK(struct MyStruct {
 *   uint32_t foo;
 *   uint32_t bar;
 * });
 */
