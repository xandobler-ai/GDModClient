#include "gdmc/GeodeIndex.hpp"
#include "gdmc/Http.hpp"

#include <iostream>

namespace gdmc {

std::vector<ModInfo> GeodeIndex::search(const std::string& query)
{
    std::cout << "[GDMC] Searching for: "
              << query << "\n";

    // Real Geode Index connection will be added here.

    return {};
}

}
