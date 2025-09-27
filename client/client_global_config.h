#pragma once

namespace tang {
namespace client {
// you can add some config to this!
struct ClientGlobalConfig {
    // the message show time! ms
    static constexpr size_t message_show_time = 1000;
    static constexpr size_t error_show_time   = 1200;
    
    static constexpr size_t button_icon_size = 16;
    static constexpr size_t mini_icon_size   = 21;
    static constexpr size_t small_icon_size  = 27;
    static constexpr size_t middle_icon_size = 36;
    static constexpr size_t large_icon_size  = 48;
};
}   // namespace client
}   // namespace tang