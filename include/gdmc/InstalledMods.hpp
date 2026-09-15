#pragma once

#include <string>
#include <vector>

namespace gdmc {

class InstalledMods {
public:
    static std::vector<std::string> scan(
        const std::string& directory
    );
};

}
