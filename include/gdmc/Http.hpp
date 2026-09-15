#pragma once

#include <string>

namespace gdmc {

class Http {
public:
    static std::string get(const std::string& url);
};

}
