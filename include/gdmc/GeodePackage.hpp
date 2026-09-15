#pragma once

#include <string>

namespace gdmc {

class GeodePackage {
public:
    static bool extract(
        const std::string& geodeFile,
        const std::string& outputDirectory
    );

    static std::string readModJson(
        const std::string& extractedDirectory
    );
};

}
