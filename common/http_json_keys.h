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

struct WorkspaceContentResponse {
    static constexpr const char* file_name_key       = "file_name";
    static constexpr const char* file_size_key       = "file_size";
    static constexpr const char* file_type_key       = "file_type";
    static constexpr const char* last_write_time_key = "last_write_time";
    static constexpr const char* file_infos_key      = "file_infos";

    static constexpr std::array<const char*, 4> item_keys = {
        file_name_key, file_size_key, file_type_key, last_write_time_key};
};

struct IsFileExistResResponse {
    static constexpr const char*                is_exist_key = "is_exist";
    static constexpr const char*                is_dir_key   = "is_dir";
    static constexpr std::array<const char*, 2> keys         = {is_exist_key, is_dir_key};
};

struct GetChunkVoteReqKeys {
    static constexpr const char*                voter_id_key    = "voter_id";
    static constexpr const char*                vote_num_key    = "vote_num";
    static constexpr const char*                vote_offset_key = "vote_offset";
    static constexpr const char*                vote_status_key = "vote_status";
    static constexpr std::array<const char*, 4> keys            = {
        voter_id_key, vote_num_key, vote_offset_key, vote_status_key};
};

struct GetParticipateVoteReqKeys : public GetChunkVoteReqKeys {
    static constexpr const char* vote_process_status_key = "vote_process_status";
};


struct GetVoteNumReqKeys {
    static constexpr const char*                voter_id_key    = "voter_id";
    static constexpr const char*                vote_status_key = "vote_status";
    static constexpr std::array<const char*, 2> keys            = {voter_id_key, vote_status_key};
};

struct GetParticipateNumReqKeys : public GetVoteNumReqKeys {
    static constexpr const char* vote_process_status_key = "vote_process_status";
};


struct SendVoteChoicesReqKeys {
    static constexpr const char* voter_id_key     = "voter_id";
    static constexpr const char* vote_choices_key = "vote_choices";
    // important!
    static constexpr const char*                vote_id_key = "vote_id";
    constexpr static std::array<const char*, 3> keys        = {
        vote_id_key, voter_id_key, vote_choices_key};
};


struct ChunkVoteReqBaseKeys {
    static constexpr const char*                vote_num_key    = "vote_num";
    static constexpr const char*                vote_offset_key = "vote_offset";
    constexpr static std::array<const char*, 2> keys            = {vote_num_key, vote_offset_key};
};
}   // namespace common
}   // namespace tang