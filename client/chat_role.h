#pragma once
#include <qnamespace.h>
namespace tang {
namespace client {
struct ChatRole {
    static constexpr int MessageTextRole  = Qt::UserRole + 1;
    static constexpr int NicknameTextRole = Qt::UserRole + 2;
    static constexpr int MessageTimeRole  = Qt::UserRole + 3;
    static constexpr int MessageWidthRole = Qt::UserRole + 4;
};
}   // namespace client
}   // namespace tang