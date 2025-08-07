cmake_minimum_required(VERSION 3.10)  # Minimum required CMake version

project(sdluiapp  # Project name
        VERSION 1.0
        LANGUAGES CXX)  # Using C++

set(CMAKE_CXX_STANDARD 17)  # Require C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the specified standard

file(GLOB T1_SRC *.h *.cpp)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${T1_SRC})

# Optional: Set compiler warning levels
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W1)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DSDL_MAIN_HANDLED)