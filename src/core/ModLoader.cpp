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

    std::cout << "[GDMC] Verifying SHA-256...\n";

    if (mod.hash.empty())
    {
        std::cerr << "[GDMC] Mod has no SHA-256 hash.\n";
        return false;
    }

    std::string actualHash =
        Hash::sha256(filename);

    if (actualHash.empty())
    {
        std::cerr << "[GDMC] Failed to calculate hash.\n";
        return false;
    }

    if (actualHash != mod.hash)
    {
        std::cerr << "[GDMC] HASH MISMATCH!\n";
        std::cerr << "[GDMC] Expected: "
                  << mod.hash
                  << "\n";

        std::cerr << "[GDMC] Actual:   "
                  << actualHash
                  << "\n";

        std::filesystem::remove(filename);

        return false;
    }

    std::cout << "[GDMC] SHA-256 verified!\n";
    std::cout << "[GDMC] Installed successfully!\n";

    return true;
}
