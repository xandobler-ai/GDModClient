#include "gdmc/Hash.hpp"
#include "gdmc/ModLoader.hpp"
#include "gdmc/Http.hpp"

#include <filesystem>
#include <iostream>

#include <windows.h>
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")

namespace gdmc {

bool ModLoader::install(const ModInfo& mod)
{
    std::cout << "[GDMC] Installing: "
              << mod.name << "\n";

    if (mod.downloadURL.empty())
    {
        std::cerr << "[GDMC] Mod has no download URL.\n";
        return false;
    }

    std::filesystem::create_directories("mods");

    std::string filename =
        "mods/" + mod.id + ".geode";

    if (!Http::download(
            mod.downloadURL,
            filename))
    {
        std::cerr << "[GDMC] Failed to install "
                  << mod.name
                  << "\n";

        return false;
    }

    std::cout << "[GDMC] Installed successfully!\n";

    return true;
}

bool ModLoader::load(const ModInfo& mod)
{
    std::cout << "[GDMC] Loading: "
              << mod.name
              << "\n";

    return true;
}

bool ModLoader::unload(const ModInfo& mod)
{
    std::cout << "[GDMC] Unloading: "
              << mod.name
              << "\n";

    return true;
}

}
