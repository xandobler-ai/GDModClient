#include "gdmc/GeodeIndex.hpp"
#include "gdmc/Http.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

namespace gdmc {

using json = nlohmann::json;

std::vector<ModInfo> GeodeIndex::search(const std::string& query)
{
    std::cout << "[GDMC] Searching Geode Index for: "
              << query << "\n";

    std::string response =
        Http::get("https://api.geode-sdk.org/v1/mods");

    if (response.empty())
    {
        std::cerr << "[GDMC] Failed to get Geode Index data.\n";
        return {};
    }

    try
    {
        json root = json::parse(response);

        std::vector<ModInfo> results;

        const auto& mods = root["payload"]["data"];

        for (const auto& mod : mods)
        {
            std::string id = mod.value("id", "");

            if (!query.empty() &&
                id.find(query) == std::string::npos)
            {
                continue;
            }

            if (!mod.contains("versions") ||
                mod["versions"].empty())
            {
                continue;
            }

            const auto& version = mod["versions"][0];

            ModInfo info;

            info.id = id;
            info.name = version.value("name", "");
            info.version = version.value("version", "");
            info.description = version.value("description", "");

            if (mod.contains("developers") &&
                !mod["developers"].empty())
            {
                info.developer =
                    mod["developers"][0].value(
                        "display_name", "");
            }

            info.downloadURL =
                version.value("download_link", "");

            info.hash =
                version.value("hash", "");

            results.push_back(info);
        }

        return results;
    }
    catch (const json::exception& error)
    {
        std::cerr << "[GDMC] JSON error: "
                  << error.what()
                  << "\n";

        return {};
    }
}

}
