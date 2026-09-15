#pragma once

#include <string>
#include <vector>

#include "ModInfo.hpp"

namespace gdmc {

class GeodeIndex {
public:
    std::vector<ModInfo> search(const std::string& query);
};

}
