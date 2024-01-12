cmake_minimum_required(VERSION 3.27)
project(tictactoe VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(PROJECT_OUTPUT_BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_OUTPUT_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_OUTPUT_BIN_DIR}")

include_directories(vendor/SFML/include)

# set(SFML_USE_STATIC_STD_LIBS TRUE)
# set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(vendor/SFML)

file(GLOB_RECURSE TTT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

file(GLOB_RECURSE TTT_ASSETS CONFIGURE_DEPENDS assets/*.png)

set(TTT_META
        "README.md"
        ".gitignore"
        ".gitmodules"
        ".clang-format"
        "LICENSE"
        "AGPLv3_Logo.svg")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TTT_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TTT_ASSETS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TTT_META})

add_executable(${PROJECT_NAME} ${TTT_SOURCES} ${TTT_ASSETS} ${TTT_META})

target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-window
        sfml-audio)
