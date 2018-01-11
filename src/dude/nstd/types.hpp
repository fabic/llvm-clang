#ifndef __NSTD_TYPES_H
#define __NSTD_TYPES_H

namespace nstd {

  /*
   * \link http://en.cppreference.com/w/cpp/language/types
   */

  // fixme?
  typedef unsigned long* uintptr_t;

  typedef unsigned long  size_t;

  /// Signed size_t
  typedef unsigned long ssize_t;

  static_assert(sizeof(void*) == 8, "Pointers should be 64-bits.");

  static_assert(sizeof(size_t) == sizeof(void*),
      "Type size_t should have the same byte-count as a pointer.");

  static_assert(sizeof(long) == sizeof(void*),
      "Integer type 'long' should have the same size as pointers.");

  static_assert(sizeof(int) == sizeof(long)/2,
      "Integer type 'int' should be “half” as 'long'.");

  static_assert(sizeof(short) == sizeof(int)/2,
      "Integer type 'short' should be “half” as 'int'.");

  static_assert(sizeof(char) == sizeof(short)/2,
      "Integer type 'char' should be “half” as 'short'.");

  using  int8_t = char ;
  using uint8_t = unsigned char ;

  using  int16_t = short ;
  using uint16_t = unsigned short ;

  using  int32_t = int ;
  using uint32_t = unsigned int ;

  using  int64_t = long ;
  using uint64_t = unsigned long ;

  using  int128_t = __int128 ;
  using uint128_t = unsigned __int128 ;

  static_assert(sizeof(int128_t) == sizeof(long)*2,
      "Integer type '__int128' should be “twice” as 'long'.");
}

#endif // __NSTD_TYPES_H

