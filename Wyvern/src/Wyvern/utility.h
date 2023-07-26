#pragma once
#include <climits>
#include <exception>

namespace Wyvern {

    //template <typename T>
    //constexpr T size_cast(size_t what) {
    //    static_assert(std::is_integral<T>::value, "Destination type must be integral.");
    //    static_assert(std::numeric_limits<T>::max() >= std::numeric_limits<size_t>::max(), "Invalid conversion - destination type not large enough to hold the value of 'size_t'.");
    //    return static_cast<T>(what);
    //}

}