#pragma once

#include <string>

namespace gdmc {

struct ModInfo {
    std::string id;
    std::string name;
    std::string version;
    std::string developer;
    std::string description;
    std::string downloadURL;
    std::string hash;
};

}
