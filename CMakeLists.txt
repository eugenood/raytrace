cmake_minimum_required(VERSION 3.23)

project(raytrace VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    if(EXISTS "/opt/homebrew/opt/libomp")
        set(OpenMP_ROOT "/opt/homebrew/opt/libomp")
    elseif(EXISTS "/usr/local/opt/libomp")
        set(OpenMP_ROOT "/usr/local/opt/libomp")
    endif()
endif()

find_package(OpenMP REQUIRED)

add_executable(raytrace
    src/main.cpp
    src/camera.cpp
    src/hittable.cpp
    src/image.cpp
    src/interval.cpp
    src/material.cpp
    src/ray.cpp
    src/util.cpp
    src/vector.cpp
)

target_include_directories(raytrace PRIVATE include)

if(OpenMP_CXX_FOUND)
    target_link_libraries(raytrace PUBLIC OpenMP::OpenMP_CXX)
endif()

target_compile_options(raytrace PRIVATE
    -Wall
    -Wextra
    -Wconversion
    -Wsign-conversion
    -Werror
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()