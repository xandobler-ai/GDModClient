#include "gdmc/Http.hpp"

#include <curl/curl.h>
#include <iostream>
#include <fstream>

namespace gdmc {

static size_t WriteString(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
)
{
    size_t totalSize = size * nmemb;

    std::string* output =
        static_cast<std::string*>(userp);

    output->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

static size_t WriteFile(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
)
{
    size_t totalSize = size * nmemb;

    std::ofstream* file =
        static_cast<std::ofstream*>(userp);

    file->write(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

std::string Http::get(const std::string& url)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "[GDMC] Failed to initialize CURL\n";
        return {};
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        std::cerr << "[GDMC] HTTP error: "
                  << curl_easy_strerror(result)
                  << "\n";

        curl_easy_cleanup(curl);
        return {};
    }

    curl_easy_cleanup(curl);

    return response;
}

bool Http::download(
    const std::string& url,
    const std::string& outputPath
)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "[GDMC] Failed to initialize CURL\n";
        return false;
    }

    std::ofstream file(
        outputPath,
        std::ios::binary
    );

    if (!file)
    {
        std::cerr << "[GDMC] Could not create file: "
                  << outputPath
                  << "\n";

        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    CURLcode result = curl_easy_perform(curl);

    file.close();
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        std::cerr << "[GDMC] Download failed: "
                  << curl_easy_strerror(result)
                  << "\n";

        return false;
    }

    std::cout << "[GDMC] Downloaded: "
              << outputPath
              << "\n";

    return true;
}

}
