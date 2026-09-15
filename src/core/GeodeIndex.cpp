#include "gdmc/GeodeIndex.hpp"

#include <iostream>

namespace gdmc {

std::vector<ModInfo> GeodeIndex::search(const std::string& query)
{
    std::cout << "[GDMC] Searching Geode Index for: "
              << query << "\n";

    // Step 5 will connect to:
    // https://api.geode-sdk.org/v1/mods

    // Actual HTTP + JSON parsing comes next.
    return {};
}

}
