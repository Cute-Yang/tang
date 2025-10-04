#pragma once
#include <array>
#include <string_view>

namespace tang {
namespace common {

enum class StatusCode : uint32_t {
    kSuccess                      = 0,
    kUsernameIsEmpty              = 1,
    kPasswordIsEmpty              = 2,
    kInvalidDatabase              = 3,
    kUsernameNotExist             = 4,
    kPasswordNotCorrect           = 5,
    kSqlRuntimeError              = 6,
    kLogicError                   = 7,
    kUsernameAlreadyExist         = 8,
    kFilePathIsEmpty              = 9,
    kFilePathIsNotFolder          = 10,
    kFilePathNotExist             = 11,
    kFileSystemRuntimeError       = 12,
    kUnexpectedAbsoluteFilePath   = 13,
    kFailToCreateFile             = 14,
    kFileAlreadyExist             = 15,
    kFailToCreateDir              = 16,
    kFailToRemoveFile             = 17,
    kUploadFilesIsEmpty           = 18,
    kJsonParamIsNull              = 19,
    kParseJsonError               = 20,
    kJsonKeyError                 = 21,
    kJsonTypeError                = 22,
    kArraySizeNotMatchError       = 23,
    kInvalidVoteid                = 24,
    kEmptyVoteChoice              = 25,
    KVoteIdNotFound               = 26,
    kVoteChoceTypeMismatch        = 27,
    kInvalidVoteChoice            = 28,
    kFailAddVoteToCache           = 29,
    kVoteCacheIsFull              = 30,
    kInvalidChoiceType            = 31,
    kVoterIsNotInclude            = 32,
    kFailRemoveVote               = 33,
    kWebSocketKeyIsAlreadyExist   = 34,
    kWebSocketConnectionIsNullptr = 35,
    kWebSocketKeyNotExsit         = 36,
    kUserisAlreadyOnline          = 37,
    kUserisInvalid                = 38,
    kDirisAlreayExit              = 39,
    kInvalidNumberError           = 40,
    Count
};

constexpr size_t status_count = static_cast<size_t>(StatusCode::Count);

constexpr std::array<std::string_view, status_count> status_strs = {
    "success!",
    "username is empty!",
    "password is empty!",
    "database is invalid!",
    "username is not exist!",
    "password is not correct!",
    "sql execute got runtime error!",
    "unexpected logic error!",
    "username is already exist!",
    "file path is empty",
    "file path is not folder!",
    "file path is not exist!",
    "filesystem runtime error!",
    "unexpected absolute file path!",
    "fail to create file!",
    "file is already exit!",
    "fail to create dir!",
    "fail to remove file!",
    "the upload files is empty!",
    "the json param is null!",
    "parse json error!",
    "json key error!",
    "unexpected json type error!",
    "the array size not match!",
    "invalid vote id!",
    "the vote choice is empty!",
    "can not find the vote id!",
    "the vote chocie type is mismatch!",
    "the vote choice is invalid",
    "fail add vote to cache!",
    "the vote cache is full!",
    "the choice type is invalid!",
    "the voter is not included!",
    "fail to remove vote!",
    "the websocket key is already exist,we only allow one person for one device now!!!",
    "the websocket connection is nullptr!!!",
    "the websocket conn key is not exist!",
    "the user is already online!",
    "the user is invalid!",
    "the dir is alreay exist!",
    "fail to convert str -> number!"};

inline constexpr std::string_view get_status_str(size_t index) {
    if (index >= status_count) {
        return std::string_view{};
    }
    return status_strs[index];
}

inline constexpr std::string_view get_status_str(StatusCode status) {
    size_t index = static_cast<size_t>(status);
    return get_status_str(index);
}

enum class FileKind : uint32_t {
    kFolder = 0,
    kPdf    = 1,
    kWord   = 2,
    kExcel  = 3,
    kPpt    = 4,
    kTxt    = 5,
    kImage  = 6,
    kPython = 7,
    kCpp    = 8,
    kOthers = 9,
    kError  = 10,
    count
};
enum class VoteChoiceType : uint8_t { kSingleChoice = 0, kMultiChoice = 1, count };
// static_assert(is_contiguous_enum_from_zeron_impl<VoteChoiceType, VoteChoiceType::kSingleChoice,
//                                                  VoteChoiceType::kMultiChoice>::value,
//               "");

constexpr size_t vote_choice_type_count = static_cast<size_t>(VoteChoiceType::count);

constexpr std::array<std::string_view, vote_choice_type_count> vote_choice_type_strs = {
    "single choice", "multi choice"};

enum class VoteStatus : uint8_t { kReady = 0, kFinished = 1, kInvalid = 2, count };

constexpr size_t vote_status_count = static_cast<size_t>(VoteStatus::count);
static_assert(vote_status_count > 0, "must be > 0!");

constexpr std::array<std::string_view, vote_status_count> vote_status_strs = {
    "ready", "finished", "invalid"};


enum class VoteUserStatus : uint8_t { kOffline = 0, kActive, kInvalid, count };
constexpr size_t vote_user_status_count = static_cast<size_t>(VoteUserStatus::count);
constexpr std::array<std::string_view, vote_user_status_count> vote_user_status_strs = {
    "offline", "active", "invalid"};


enum class VoteProcessStatus : uint8_t { kReady = 0, kProcessed = 1, kAbstained, count };

enum class VotePriority : uint8_t {
    // don't have vote priority
    kInvalid = 0,
    // normal priorty
    kNormal = 1,
    // the maximum priority!!!
    kMaximum = (1 << 8) - 1
};

// make sure encode with utf8
// struct FileInfo {
//     std::string file_name;
//     std::string modify_time;
//     size_t      file_size;
//     FileKind    file_kind;
// };

constexpr std::string_view public_workspace_name = "公共空间";

}   // namespace common
}   // namespace tang