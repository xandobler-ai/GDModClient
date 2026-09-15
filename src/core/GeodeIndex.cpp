#include "gdmc/GeodeIndex.hpp"

#include <iostream>

namespace gdmc {

std::vector<ModInfo> GeodeIndex::search(const std::string& query)
{
    std::cout << "[GDMC] Searching Geode Index for: "
              << query << "\n";

    // Real Geode Index connection will be added next.
    return {};
}

}
