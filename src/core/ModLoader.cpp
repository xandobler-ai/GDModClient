#include "gdmc/ModLoader.hpp"

#include <iostream>

namespace gdmc {

bool ModLoader::install(const ModInfo& mod)
{
    std::cout << "[GDMC] Installing: "
              << mod.name << "\n";

    // Downloading the .geode will be added later.

    return true;
}

bool ModLoader::load(const ModInfo& mod)
{
    std::cout << "[GDMC] Loading: "
              << mod.name << "\n";

    return true;
}

bool ModLoader::unload(const ModInfo& mod)
{
    std::cout << "[GDMC] Unloading: "
              << mod.name << "\n";

    return true;
}

}
