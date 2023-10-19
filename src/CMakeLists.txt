
include_directories(${LearnSDL2_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCEFILES "*.cpp")
foreach(_source IN LISTS SOURCEFILES)
    get_filename_component(_basename ${_source} NAME_WE)
    add_executable(${_basename} WIN32  ${_source})
    target_link_libraries(${_basename} PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image)
endforeach(_source IN LISTS SOURCEFILES)