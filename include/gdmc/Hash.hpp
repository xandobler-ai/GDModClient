#pragma once

#include <string>

namespace gdmc {

class Hash {
public:
    static std::string sha256(const std::string& filePath);
};

}
