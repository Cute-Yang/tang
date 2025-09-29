#include "response_validator.h"
#include "common/response_keys.h"
#include <QJsonArray>

using namespace tang::common;

namespace tang {
namespace client {

bool WorkspaceResJsonValidator::validate(const QJsonObject& json_data) {
    constexpr auto keys = WorkspaceResJsonKeys::keys;
    for (size_t i = 0; i < keys.size(); ++i) {
        if (!json_data.contains(keys[i])) {
            return false;
        }
        // validate type!
        if (!json_data[keys[i]].isArray()) {
            return false;
        }
        auto values = json_data[keys[i]].toArray();
        for (auto value : values) {
            if (!value.isString()) {
                break;
            }
        }
    }

    return true;
}


bool WorkspaceContentResJsonValidator::validate(const QJsonObject& json_data) {
    if (!json_data.contains(WorkspaceContentResponse::file_infos_key)) {
        return false;
    }

    auto file_infos_json = json_data[WorkspaceContentResponse::file_infos_key];
    if (!file_infos_json.isArray()) {
        return false;
    }
    auto file_infos = file_infos_json.toArray();
    for (size_t i = 0; i < file_infos.size(); ++i) {
        auto item = file_infos[i];
        if (!item.isObject()) {
            return false;
        }
        // then validate keys
        constexpr auto keys = WorkspaceContentResponse::item_keys;
        for (size_t j = 0; j < keys.size(); ++j) {
            if (!item.toObject().contains(keys[j])) {
                return false;
            }
        }
        // then validate the type!
        if (!item.toObject()[WorkspaceContentResponse::file_name_key].isString() ||
            !item.toObject()[WorkspaceContentResponse::file_size_key].isDouble() ||
            !item.toObject()[WorkspaceContentResponse::file_type_key].isDouble() ||
            !item.toObject()[WorkspaceContentResponse::last_write_time_key].isString()) {
            return false;
        }
    }

    return true;
}

bool IsFileExistResJsonValidator::validate(const QJsonObject& json_data) {
    for (auto& key : IsFileExistResResponse::keys) {
        if (!(json_data.contains(key) && json_data[key].isBool())) {
            return false;
        }
    }
    return true;
}

}   // namespace client
}   // namespace tang