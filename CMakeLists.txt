cmake_minimum_required(VERSION 3.24)
project(tictactoe VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(PROJECT_OUTPUT_BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_OUTPUT_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_OUTPUT_BIN_DIR}")

include_directories(vendor/SFML/include)

set(SFML_USE_STATIC_STD_LIBS TRUE)
set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(vendor/SFML)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-window
        sfml-graphics
        sfml-system
        sfml-audio)
