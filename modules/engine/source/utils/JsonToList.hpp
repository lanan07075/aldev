//
// Created by dawn on 2025/9/3.
//

#ifndef AGNET_JSONTOLIST_HPP
#define AGNET_JSONTOLIST_HPP

#include <iostream>
#include <list>
#include <string>

#include "json/json.hpp"

static std::list<std::string> JsonToList(nlohmann::json paths){
    std::list<std::string> plugin_paths;
    for (const auto& item : paths) {
        if (item.is_string()) {
            plugin_paths.push_back(item.get<std::string>());
        } else {
            std::cerr << "Warning: Non-string element found and skipped." << std::endl;
        }
    }
    return plugin_paths;
}

#endif //AGNET_JSONTOLIST_HPP
