
include_directories(${LearnSDL2_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCEFILES "*.cpp")
foreach(_source IN LISTS SOURCEFILES)
    get_filename_component(_target_name ${_source} NAME_WE)
    add_executable(${_target_name} WIN32  ${_source})
    target_link_libraries(${_target_name} PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
    add_dependencies(${_target_name} copy_all_)
endforeach(_source IN LISTS SOURCEFILES)