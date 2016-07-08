#ifndef FABIC_XCB_MASK_VALUES_H
#define FABIC_XCB_MASK_VALUES_H

#include <array>

#include "fabic/xcb+/typedefs.hpp"
#include "fabic/util/bits.hpp"


namespace fabic {
namespace xcb {

  using fabic::util::is_power_of_two;
  using fabic::util::popcount;
  using std::array;

  template<
      class EnumType,
      uint32_t Bitmask = 0,
      uint32_t Size = popcount<uint32_t>(Bitmask)
    >
    class MaskValues
    {
    public:
      uint32_t bitmask = Bitmask;
      array<uint32_t, Size> values;

      MaskValues() { }

      uint32_t getBitmask() { return this->bitmask; }

      uint32_t operator[](uint32_t bit)
      {
        // assert( is_power_of_two(bit) );
        // assert( (bit & this->bitmask) != 0 ); // bit must be set.
        // uint32_t bit_index = popcount<uint32_t>(
        //     bit ^ (~(bit-1) & this->bitmask
        //   );
      }
    };

} // xcb ns
} // fabic ns

#endif // FABIC_XCB_MASK_VALUES_H
