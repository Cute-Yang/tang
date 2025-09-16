#include <iostream>
#include <json/json.h>
#include <string>



void parse_json_string() {
    std::string raw_json_str = R"json(
    {
        "city_name":"seattle",
        "is_prefered":true,
        "beside_sea":true,
        "lattitude":102.34,
        "population":230
    }
    )json";

    Json::Value                             parsed_result;
    JSONCPP_STRING                          err;
    Json::CharReaderBuilder                 builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(raw_json_str.c_str(),
                       raw_json_str.c_str() + raw_json_str.size(),
                       &parsed_result,
                       &err)) {
        std::cout << "fail to parse json,error:" << err << std::endl;
        return;
    }

    std::string city_name  = parsed_result["city_name"].asString();
    bool        beside_sea = parsed_result["beside_sea"].asBool();
    float       lattitude  = parsed_result["lattitude"].asFloat();

    std::cout << "city name:" << city_name << std::endl;
    std::cout << "besie sea:" << beside_sea << std::endl;
    std::cout << "lattitude:" << lattitude << std::endl;
}

int main() {
    parse_json_string();
    return 0;
}