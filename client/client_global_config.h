#pragma once

namespace tang {
namespace client {
// you can add some config to this!
struct ClientGlobalConfig {
    // the message show time!
    static constexpr size_t message_show_time = 1800;
    static constexpr size_t error_show_time   = 2700;
};
}   // namespace client
}   // namespace tang