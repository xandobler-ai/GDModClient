cmake_minimum_required(VERSION 3.20)

project(GDModClient
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# -------------------------
# CURL
# -------------------------

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_16_0
)

set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "" FORCE)
set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBIDN2 OFF CACHE BOOL "" FORCE)
set(CURL_USE_GSSAPI OFF CACHE BOOL "" FORCE)
set(USE_NGHTTP2 OFF CACHE BOOL "" FORCE)

set(HTTP_ONLY ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(curl)

# -------------------------
# JSON
# -------------------------

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

# -------------------------
# GDModClient
# -------------------------

add_executable(GDModClient
    src/main.cpp
    src/core/ModLoader.cpp
    src/core/GeodeIndex.cpp
    src/core/Http.cpp
    src/core/Hash.cpp
    src/core/InstalledMods.cpp
    src/core/GeodePackage.cpp
)

target_include_directories(GDModClient PRIVATE
    include
)

target_link_libraries(GDModClient PRIVATE
    libcurl
    nlohmann_json::nlohmann_json
)
