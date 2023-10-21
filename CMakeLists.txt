cmake_minimum_required(VERSION 3.11)
project(LearnSDL2)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_subdirectory(src)

set(imageSource "${CMAKE_SOURCE_DIR}/images")
set(imageDestination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/images")
add_custom_target(
    copyImages
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${imageSource} ${imageDestination} 
    DEPENDS ${imageSource}
)


set(fontSource "${CMAKE_SOURCE_DIR}/fonts")
set(fontDestination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/fonts")
add_custom_target(
    copyFonts
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${fontSource} ${fontDestination} 
    DEPENDS ${fontDestination}
)