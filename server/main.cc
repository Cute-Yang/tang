#include "controllers/util.h"
#include <drogon/drogon.h>
#include <filesystem>


int main(int argc, char* argv[]) {
    LOG_INFO
        << "when you use the windows,if contain the chinese charactor,you'd better set chcp 65001";
    if (argc != 3) {
        LOG_INFO << "Usage " << argv[0] << " 'config_file'" << " 'workspace_root'";
        return -1;
    }
    std::string config_json_file = argv[1];
    std::string workspace_root   = argv[2];
    LOG_INFO << "the config file is " << argv[1] << "the workspace root is " << workspace_root;
    tang::server::utils::get_db_client_name() = "default";
    tang::server::utils::get_workspace_root() = workspace_root;
    tang::server::utils::try_to_create_workspace(std::string(tang::common::public_workspace_name));

    if (!std::filesystem::exists(config_json_file)) {
        LOG_ERROR << config_json_file << " is not exist,please specify a valid config file..";
        return -1;
    }
    LOG_INFO << "load config from file " << config_json_file;
    drogon::app().loadConfigFile(config_json_file);
    drogon::app().run();
    return 0;
}
