#ifndef DUDE_UTIL_BITS_H
#define DUDE_UTIL_BITS_H

#include <cstdint>
#include <type_traits>

namespace dude {
namespace util {

  /**
   * http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
   *
   * todo: negative signed integers ?
   */
  template<typename Int>
    constexpr bool is_power_of_two(Int x)
    {
      typename std::make_unsigned<Int>::type _x = x;
      return ((_x != 0) && !(_x & (_x - 1)));
    }

  /**
   * @link http://en.cppreference.com/w/cpp/utility/bitset/count
   * @link http://stackoverflow.com/a/11816547
   *
   *   “ This algorithm is based on Divide and Conquer Algorithm. Suppose there is a 8bit integer 213(11010101 in binary), the algorithm works like this(each time merge two neighbor blocks):
   *
   *     +-------------------------------+
   *     | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |  <- x
   *     |  1 0  |  0 1  |  0 1  |  0 1  |  <- first time merge
   *     |    0 0 1 1    |    0 0 1 0    |  <- second time merge
   *     |        0 0 0 0 0 1 0 1        |  <- third time ( answer = 00000101 = 5)
   *     +-------------------------------+
   *    ”
   *
   * @param bitmask
   *
   * @return the number of set bits in the bitmask integer.
   */
  template<typename IntegerType>
  constexpr IntegerType popcount(IntegerType bits)
  {
    typename std::make_unsigned<IntegerType>::type x = bits;

    // This code only works for 32-bits and smaller integer types.
    static_assert( sizeof(x) <= sizeof(uint32_t) );

    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
    x = (x & 0x0000FFFF) + ((x >> 16)& 0x0000FFFF);

    return x;
  }

  static_assert( popcount(128 | 64 | 32 | 16 | 8 | 4 | 2 | 1) == 8 );
  static_assert( popcount(128 | 32 | 8 | 2) == 4 );
  static_assert( popcount( 32 | 16 | 4 | 1) == 4 );
  static_assert( popcount(255) == 8 );
  static_assert( popcount(127) == 7 );
  static_assert( popcount( 63) == 6 );
  static_assert( popcount( 31) == 5 );
  static_assert( popcount( 15) == 4 );
  static_assert( popcount(  7) == 3 );
  static_assert( popcount(  3) == 2 );
  static_assert( popcount(  1) == 1 );
  static_assert( popcount( -1) == 32);
  static_assert( popcount( -2) == 31);


  /**
   * Return the index of `bit` wrt. the bits that are set in the `bitmask`.
   */
  template<typename Int>
  constexpr Int bit_index(Int bit, Int bitmask)
  {
    assert( is_power_of_two(bit) );
    assert( (bit & bitmask) != 0 ); // bit must be set.

    uint32_t bit_index = popcount<uint32_t>(
        bit ^ ((bit-1) & bitmask)
      );

    return bit_index;
  }

  static_assert( bit_index<uint32_t>(8,       8|4|2|1) == 4 );
  static_assert( bit_index<uint32_t>(8,       8|4|2  ) == 3 );
  static_assert( bit_index<uint32_t>(8,       8|  2|1) == 3 );
  static_assert( bit_index<uint32_t>(8, 32|16|8|4|2|1) == 4 );
  static_assert( bit_index<uint32_t>(8, 32|   8|4|2|1) == 4 );
  static_assert( bit_index<uint32_t>(8, 32|   8|4|  1) == 3 );

} // util ns.
} // dude ns

#endif // DUDE_UTIL_BITS_H
