cmake_minimum_required(VERSION 3.30)
project(Rendering LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
# -Werror will refuse to compile if there are any warnings, if this becomes problematic remove it.
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")


# Fetch required libraries for building.
include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glbinding
        GIT_REPOSITORY https://github.com/cginternals/glbinding.git
        GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(glbinding)
option(OPTION_BUILD_OWN_KHR_HEADERS "Forces GLBinding to use an internal Khronos header" ON)


# Platform agnostic include and source files.
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/libs
)

set(SOURCE_FILES
        src/main.cpp
        libs/stb_image/stb_image.cpp
        src/shader.cpp
)

# Windows specific source files and includes.
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include_directories(include/platform/windows)

    set(SOURCE_FILES ${SOURCE_FILES}

    )
endif()

# Linux specific source files and includes.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include_directories(include/platform/linux)

    set(SOURCE_FILES ${SOURCE_FILES}

    )
endif()


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE
        glm::glm
        glbinding::glbinding
        glfw
)
