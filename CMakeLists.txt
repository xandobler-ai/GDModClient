cmake_minimum_required(VERSION 3.20)

project(GDModClient
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GDModClient
    src/main.cpp
    src/core/ModLoader.cpp
)
)

target_include_directories(GDModClient PRIVATE
    include
)
