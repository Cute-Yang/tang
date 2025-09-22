#include "util.h"
#include "common/response_keys.h"
#include "drogon/utils/Utilities.h"


using namespace tang::common;
namespace tang {
namespace server {
namespace utils {
constexpr std::array<std::string_view, 1> pdf_exts   = {"ext"};
constexpr std::array<std::string_view, 4> word_exts  = {"docx", "doc", "docm", "dotx"};
constexpr std::array<std::string_view, 6> excel_exts = {
    "xlsx", "xlsm", "xltx", "xltm", "xls", "csv"};
constexpr std::array<std::string_view, 5> ppt_exts = {"pptx", "ppt", "pptm", "potx", "potm"};

constexpr std::array<std::string_view, 1> txt_exts = {"txt"};

constexpr std::array<std::string_view, 5> image_exts = {"jpg", "jpeg", "png", "tif", "tiff"};

constexpr std::array<std::string_view, 1> py_exts = {"py"};

constexpr std::array<std::string_view, 4> cpp_exts = {"cc", "cxx", "cpp", "ixx"};

bool is_ext_found(const std::string_view* exts, size_t n, std::string_view ext) {
    for (size_t i = 0; i < n; ++i) {
        if (ext == exts[i]) {
            return true;
        }
    }
    return false;
}


Json::Value make_json_from_status_code(common::StatusCode status) {
    Json::Value ret;
    ret[PublicResponseJsonKeys::status_key]  = static_cast<int>(status);
    ret[PublicResponseJsonKeys::message_key] = tang::common::get_status_str(status).data();
    return ret;
}

void make_response_from_status_code(common::StatusCode status, const callback_func& callback) {
    auto ret  = make_json_from_status_code(status);
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

std::filesystem::path& get_workspace_root() {
    static std::filesystem::path workspace_root = "";
    return workspace_root;
}

std::string& get_db_client_name() {
    static std::string db_client_name = "default";
    return db_client_name;
}

void to_lower(char* s, size_t len) {
    constexpr char delta = 'a' - 'A';
    for (size_t i = 0; i < len; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + delta;
        }
    }
}


common::FileKind get_file_kind(const std::filesystem::path& fp) {
    // only file_entry has the file...
    std::error_code ec;
    bool            is_dir = std::filesystem::is_directory(fp, ec);
    if (ec) {
        return common::FileKind::kError;
    }
    if (is_dir) {
        return common::FileKind::kFolder;
    }
    ec.clear();
    bool is_file = std::filesystem::is_regular_file(fp, ec);
    if (ec) {
        return common::FileKind::kError;
    }
    if (is_file) {
        // to utf-8
        auto ext = drogon::utils::fromNativePath(fp.extension().native());
        // convert it -> lower!
        to_lower(ext.data(), ext.size());
        common::FileKind file_kind;
        if (is_ext_found(pdf_exts.data(), pdf_exts.size(), ext)) {
            file_kind = common::FileKind::kPdf;
        } else if (is_ext_found(word_exts.data(), word_exts.size(), ext)) {
            file_kind = common::FileKind::kWord;
        } else if (is_ext_found(excel_exts.data(), excel_exts.size(), ext)) {
            file_kind = common::FileKind::kExcel;
        } else if (is_ext_found(ppt_exts.data(), ppt_exts.size(), ext)) {
            file_kind = common::FileKind::kPpt;
        } else if (is_ext_found(txt_exts.data(), txt_exts.size(), ext)) {
            file_kind = common::FileKind::kTxt;
        } else if (is_ext_found(cpp_exts.data(), cpp_exts.size(), ext)) {
            file_kind = common::FileKind::kCpp;
        }
    }
    return common::FileKind::kOthers;
}

std::string get_file_last_time_str(const std::filesystem::directory_entry& p) {
    auto file_time = p.last_write_time();
    auto sys_time  = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        file_time - std::filesystem::file_time_type::clock::now() +
        std::chrono::system_clock::now());
    std::time_t tt = std::chrono::system_clock::to_time_t(sys_time);

    // 使用 localtime_s (Windows) 或 localtime_r (Linux) 进行线程安全转换
    std::tm tm_snapshot;
#ifdef _WIN32
    localtime_s(&tm_snapshot, &tt);
#else
    localtime_r(&tt, &tm_snapshot);   // POSIX
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_snapshot, "%Y-%m-%d %H:%M:%S");   // 格式：2025-09-18 20:52:00
    return oss.str();
}


[[nodiscard]]
common::StatusCode get_full_path(const std::string& file_path, std::filesystem::path& full_path) {
    if (file_path.empty()) {
        return common::StatusCode::kFilePathIsEmpty;
    }

    if (file_path.starts_with("/") || file_path.starts_with('\\')) {
        // invalid!
        LOG_ERROR << "Path " << file_path << "  starts with root which is not allowed!";
        return common::StatusCode::kUnexpectedAbsoluteFilePath;
    }

    full_path =
        get_workspace_root() / std::filesystem::path(drogon::utils::toNativePath(file_path));
    return common::StatusCode::kSuccess;
}

}   // namespace utils
}   // namespace server
}   // namespace tang