#include <iostream>

#include "gdmc/ModInfo.hpp"
#include "gdmc/ModLoader.hpp"

int main()
{
    std::cout << "============================\n";
    std::cout << "       GD Mod Client\n";
    std::cout << "============================\n\n";

    gdmc::ModInfo testMod;

    testMod.id = "example.testmod";
    testMod.name = "Example Mod";
    testMod.version = "1.0.0";
    testMod.developer = "GDMC";
    testMod.description = "A test mod.";

    gdmc::ModLoader loader;

    std::cout << "Testing mod system...\n\n";

    loader.install(testMod);
    loader.load(testMod);
    loader.unload(testMod);

    std::cout << "\nMod system test complete!\n";

    return 0;
}
