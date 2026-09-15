#include "gdmc/Http.hpp"

#include <iostream>

namespace gdmc {

std::string Http::get(const std::string& url)
{
    std::cout << "[GDMC] HTTP GET: " << url << "\n";

    // Networking implementation will be added here.
    return {};
}

}
