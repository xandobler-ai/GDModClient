#include "gdmc/Hash.hpp"

#include <windows.h>
#include <bcrypt.h>

#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

namespace gdmc {

std::string Hash::sha256(const std::string& filePath)
{
    std::ifstream file(
        filePath,
        std::ios::binary
    );

    if (!file)
        return {};

    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;

    if (BCryptOpenAlgorithmProvider(
            &algorithm,
            BCRYPT_SHA256_ALGORITHM,
            nullptr,
            0) != 0)
    {
        return {};
    }

    DWORD objectSize = 0;
    DWORD dataSize = 0;

    BCryptGetProperty(
        algorithm,
        BCRYPT_OBJECT_LENGTH,
        reinterpret_cast<PUCHAR>(&objectSize),
        sizeof(objectSize),
        &dataSize,
        0
    );

    std::vector<unsigned char> hashObject(objectSize);

    if (BCryptCreateHash(
            algorithm,
            &hash,
            hashObject.data(),
            objectSize,
            nullptr,
            0,
            0) != 0)
    {
        BCryptCloseAlgorithmProvider(
            algorithm,
            0
        );

        return {};
    }

    std::vector<char> buffer(8192);

    while (file)
    {
        file.read(
            buffer.data(),
            buffer.size()
        );

        std::streamsize bytesRead =
            file.gcount();

        if (bytesRead > 0)
        {
            if (BCryptHashData(
                    hash,
                    reinterpret_cast<PUCHAR>(
                        buffer.data()),
                    static_cast<ULONG>(bytesRead),
                    0) != 0)
            {
                BCryptDestroyHash(hash);
                BCryptCloseAlgorithmProvider(
                    algorithm,
                    0
                );

                return {};
            }
        }
    }

    unsigned char digest[32];

    if (BCryptFinishHash(
            hash,
            digest,
            sizeof(digest),
            0) != 0)
    {
        BCryptDestroyHash(hash);
        BCryptCloseAlgorithmProvider(
            algorithm,
            0
        );

        return {};
    }

    BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(
        algorithm,
        0
    );

    std::ostringstream result;

    result << std::hex
           << std::setfill('0');

    for (unsigned char byte : digest)
    {
        result << std::setw(2)
               << static_cast<int>(byte);
    }

    return result.str();
}

}
