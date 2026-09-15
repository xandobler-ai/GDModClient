#include <iostream>
#include <string>

#include "gdmc/InstalledMods.hpp"
#include "gdmc/GeodeIndex.hpp"
#include "gdmc/ModLoader.hpp"

int main()
{
    std::cout << "============================\n";
    std::cout << "       GD Mod Client\n";
    std::cout << "============================\n\n";

    gdmc::GeodeIndex index;
    gdmc::ModLoader loader;

    std::string query;

    std::cout << "Enter a mod to search for: ";
    std::getline(std::cin, query);

    auto results = index.search(query);

    std::cout << "\nFound "
              << results.size()
              << " mod(s).\n\n";

    if (results.empty())
    {
        return 0;
    }

    for (size_t i = 0; i < results.size(); ++i)
    {
        const auto& mod = results[i];

        std::cout << "[" << i + 1 << "] "
                  << mod.name
                  << " v" << mod.version
                  << "\n";

        std::cout << "    Developer: "
                  << mod.developer
                  << "\n";

        std::cout << "    "
                  << mod.description
                  << "\n\n";
    }

    std::cout << "Enter the number of the mod to install: ";

    int choice;
    std::cin >> choice;

    if (choice < 1 ||
        choice > static_cast<int>(results.size()))
    {
        std::cout << "Invalid choice.\n";
        return 0;
    }

    const auto& selected =
        results[choice - 1];

    std::cout << "\nInstalling "
              << selected.name
              << "...\n\n";

    loader.install(selected);

    return 0;
}
