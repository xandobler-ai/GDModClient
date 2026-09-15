#include "gdmc/InstalledMods.hpp"

#include <filesystem>

namespace gdmc {

std::vector<std::string> InstalledMods::scan(
    const std::string& directory)
{
    std::vector<std::string> mods;

    if (!std::filesystem::exists(directory))
        return mods;

    for (const auto& entry :
         std::filesystem::directory_iterator(directory))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() == ".geode")
        {
            mods.push_back(
                entry.path().string()
            );
        }
    }

    return mods;
}

}
