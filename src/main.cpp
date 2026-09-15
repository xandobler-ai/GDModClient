#include <iostream>
#include <string>

#include "gdmc/GeodeIndex.hpp"

int main()
{
    std::cout << "============================\n";
    std::cout << "       GD Mod Client\n";
    std::cout << "============================\n\n";

    gdmc::GeodeIndex index;

    std::string query;

    std::cout << "Enter a mod to search for: ";
    std::getline(std::cin, query);

    auto results = index.search(query);

    std::cout << "\nFound "
              << results.size()
              << " mod(s).\n\n";

    for (const auto& mod : results)
    {
        std::cout << "----------------------------\n";
        std::cout << mod.name << "\n";
        std::cout << "ID: " << mod.id << "\n";
        std::cout << "Version: " << mod.version << "\n";
        std::cout << "Developer: " << mod.developer << "\n";
        std::cout << "Description: "
                  << mod.description << "\n";
        std::cout << "Download: "
                  << mod.downloadURL << "\n";
    }

    return 0;
}
