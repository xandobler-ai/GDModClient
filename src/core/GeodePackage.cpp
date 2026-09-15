#include "gdmc/GeodePackage.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace gdmc {

bool GeodePackage::extract(
    const std::string& geodeFile,
    const std::string& outputDirectory)
{
    if (!std::filesystem::exists(geodeFile))
    {
        std::cerr << "[GDMC] Package does not exist: "
                  << geodeFile << "\n";

        return false;
    }

    std::filesystem::create_directories(
        outputDirectory
    );

    // Windows PowerShell can extract ZIP archives.
    // A .geode package uses the same ZIP container format.

    std::string command =
        "powershell.exe -NoProfile -Command "
        "\"Expand-Archive -LiteralPath '" +
        geodeFile +
        "' -DestinationPath '" +
        outputDirectory +
        "' -Force\"";

    int result = std::system(command.c_str());

    if (result != 0)
    {
        std::cerr << "[GDMC] Failed to extract package.\n";
        return false;
    }

    std::cout << "[GDMC] Extracted: "
              << geodeFile
              << "\n";

    return true;
}

std::string GeodePackage::readModJson(
    const std::string& extractedDirectory)
{
    std::filesystem::path path =
        std::filesystem::path(extractedDirectory)
        / "mod.json";

    std::ifstream file(path);

    if (!file)
    {
        std::cerr << "[GDMC] mod.json not found.\n";
        return {};
    }

    std::stringstream contents;
    contents << file.rdbuf();

    return contents.str();
}

}
