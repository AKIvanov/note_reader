cmake_minimum_required(VERSION 3.16)
project(note_reader LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build the single executable from your existing file
add_executable(note_reader note_reader.cpp)

# Optional: per-config flags (tweak as you like)
if(NOT CMAKE_CONFIGURATION_TYPES)  # single-config generators
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g")