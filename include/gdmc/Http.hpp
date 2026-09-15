#pragma once

#include <string>

namespace gdmc {

class Http {
public:
    static std::string get(const std::string& url);

    static bool download(
        const std::string& url,
        const std::string& outputPath
    );
};

}
