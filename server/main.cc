#include "controllers/util.h"
#include <drogon/drogon.h>
#include <filesystem>


int main() {
    // Set HTTP listener address and port
    //  drogon::app().addListener("0.0.0.0", 5555);
    std::string config_json_file = "D:/work/tang/server/config.json";

    tang::server::utils::get_db_client_name() = "default";
    tang::server::utils::get_workspace_root() = "D:/";


    if (!std::filesystem::exists(config_json_file)) {
        LOG_ERROR << config_json_file << " is not exist,please specify a valid config file..";
        return -1;
    }
    LOG_INFO << "load config from file " << config_json_file;
    drogon::app().loadConfigFile(config_json_file);
    // Load config file
    // drogon::app().loadConfigFile("../config.json");
    // drogon::app().loadConfigFile("../config.yaml");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
