#ifndef FABIC_XCB_MASK_VALUES_H
#define FABIC_XCB_MASK_VALUES_H

#include <array>

# include "fabic/surface/xcb/typedefs.hpp"
# include "fabic/util/bits.hpp"

namespace sf {
  namespace xcb {

    using TLNS::util::is_power_of_two;
    using TLNS::util::popcount;
    using TLNS::util::bit_index;
    using std::array;

    template< uint32_t Bitmask,
        typename IntegerType = uint32_t,
        uint32_t Size = popcount<uint32_t>(Bitmask) >
    class MaskValues
    {
    public:
      uint32_t              _bitmask = Bitmask;
      array<IntegerType, Size> _values;

      MaskValues() { }

      uint32_t bitmask() const { return this->_bitmask; }

      uint32_t size() const { return this->_values.size(); }

      IntegerType* data() { return this->_values.data(); }

      uint32_t& operator[](uint32_t bit)
      {
        auto index = bit_index(bit, this->_bitmask) - 1;
        return this->_values[ index ];
      }
    };

  } // xcb ns.
} // sf ns.

#endif // FABIC_XCB_MASK_VALUES_H
