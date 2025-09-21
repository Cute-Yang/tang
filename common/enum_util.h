#include <algorithm>

namespace server {
namespace utils {
template<class Enum, Enum... Values> struct is_contiguous_enum_from_zeron_impl {
    static constexpr bool value = []() {
        constexpr std::size_t n      = sizeof...(Values);
        std::array<Enum, n>   sorted = {Values...};
        std::sort(sorted.begin(), sorted.end());
        for (size_t i = 0; i < n; ++i) {
            if (i != static_cast<size_t>(sorted[i])) {
                return false;
            }
        }
        return true;
    };
};
}   // namespace utils
}   // namespace server