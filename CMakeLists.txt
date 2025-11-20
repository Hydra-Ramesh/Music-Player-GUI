cmake_minimum_required(VERSION 3.10)
project(MusicPlayer)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# ImGui
set(IMGUI_DIR "vendor/imgui")
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp"
)
include_directories(${IMGUI_DIR} "${IMGUI_DIR}/backends")

# Miniaudio
include_directories("vendor/miniaudio")

# Source files
file(GLOB SOURCES "src/*.cpp" "src/*.h")

add_executable(MusicPlayer MACOSX_BUNDLE ${SOURCES} ${IMGUI_SOURCES})
target_link_libraries(MusicPlayer ${SDL2_LIBRARIES})

# macOS Bundle Properties
if(APPLE)
    set_target_properties(MusicPlayer PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Music Player"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.musicplayer"
        MACOSX_BUNDLE_ICON_FILE "AppIcon.icns"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    )
endif()
