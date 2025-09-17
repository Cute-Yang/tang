#pragma once

namespace tang {
namespace common {
enum class StatusCode {
    Success = 0,
    UsernameIsEmpty= 1,
    PasswordIsEmpty = 2,
    InvalidDatabase = 3,
    UsernameNotExist =4,
    PasswordNotCorrect =5,
    SqlRuntimeError = 6,
    LogicError = 7,
};
}
}   // namespace tang