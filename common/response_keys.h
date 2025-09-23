#pragma once
#include <array>

namespace tang {
namespace common {
struct PublicResponseJsonKeys {
    static constexpr const char* status_key  = "status";
    static constexpr const char* message_key = "message";
};


struct LoginResponseJsonKeys {
    static constexpr const char* user_id_key       = "user_id";
    static constexpr const char* email_key         = "email";
    static constexpr const char* user_name_key     = "user_name";
    static constexpr const char* vote_priority_key = "vote_priority";

    static constexpr std::array<const char*, 4> keys = {
        user_id_key, email_key, user_name_key, vote_priority_key};
};

struct WorkspaceResJsonKeys {
    static constexpr const char*                workspaces_key           = "workspaces";
    static constexpr const char*                workspace_show_names_key = "workspace_show_names";
    static constexpr std::array<const char*, 2> keys = {workspaces_key, workspace_show_names_key};
};
}   // namespace common
}   // namespace tang