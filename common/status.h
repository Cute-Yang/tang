#pragma once
#include <array>
#include <string>
#include <string_view>

namespace tang {
namespace common {
enum class StatusCode {
    kSuccess                     = 0,
    kUsernameIsEmpty             = 1,
    kPasswordIsEmpty             = 2,
    kInvalidDatabase             = 3,
    kUsernameNotExist            = 4,
    kPasswordNotCorrect          = 5,
    kSqlRuntimeError             = 6,
    kLogicError                  = 7,
    kUsernameAlreayExit          = 8,
    kFilePathIsEmpty             = 9,
    kFilePathIsNotFolder         = 10,
    kFilePathNotExist            = 11,
    kFileSystemRuntimeError      = 12,
    kUnexpectedAbasoluteFilePath = 13,
    kFailToCreateFile            = 13,
    kFileAlreadyExit = 14,
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
    "fail to create file!",
"file is already exit!"};

inline constexpr std::string_view get_status_str(StatusCode status) {
    size_t index = static_cast<size_t>(status);
    if (index >= status_count) {
        return std::string_view{};
    }
    return status_strs[index];
}

enum class FileKind {
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
    count
};




// make sure encode with utf8
// struct FileInfo {
//     std::string file_name;
//     std::string modify_time;
//     size_t      file_size;
//     FileKind    file_kind;
// };


}   // namespace common
}   // namespace tang