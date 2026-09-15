#pragma once

#include "ModInfo.hpp"

namespace gdmc {

class ModLoader {
public:
    bool install(const ModInfo& mod);
    bool load(const ModInfo& mod);
    bool unload(const ModInfo& mod);
};

}
