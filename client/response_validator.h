#pragma once
#include <QJsonObject>
namespace tang {
namespace client {
struct WorkspaceResJsonValidator {
    static bool validate(const QJsonObject& json_data);
};

struct WorkspaceContentResJsonValidator {
    static bool validate(const QJsonObject& json_data);
};

struct IsFileExistResJsonValidator {
    static bool validate(const QJsonObject& json_data);
};
}   // namespace client
}   // namespace tang