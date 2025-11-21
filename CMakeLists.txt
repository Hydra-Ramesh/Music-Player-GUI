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

if(APPLE)
    add_executable(MusicPlayer MACOSX_BUNDLE ${SOURCES} ${IMGUI_SOURCES})
    target_link_libraries(MusicPlayer ${SDL2_LIBRARIES})
    set_target_properties(MusicPlayer PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Music Player"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.musicplayer"
        MACOSX_BUNDLE_ICON_FILE "AppIcon.icns"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    )
elseif(WIN32)
    add_executable(MusicPlayer WIN32 ${SOURCES} ${IMGUI_SOURCES})
    target_link_libraries(MusicPlayer ${SDL2_LIBRARIES} -lmingw32 -lSDL2main -lSDL2)
else()
    add_executable(MusicPlayer ${SOURCES} ${IMGUI_SOURCES})
    target_link_libraries(MusicPlayer ${SDL2_LIBRARIES})
endif()

# Install configuration
install(TARGETS MusicPlayer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# CPack Configuration
set(CPACK_PACKAGE_NAME "MusicPlayer")
set(CPACK_PACKAGE_VENDOR "CodeAndCoffee")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple C++ Music Player")

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "Music Player")
elseif(WIN32)
    set(CPACK_GENERATOR "ZIP")
    
    # Bundle SDL2.dll for Windows
    # This assumes SDL2.dll is in the same directory as the library or in a standard bin path
    # A more robust way would be to use a custom command or BundleUtilities, but for now we'll try to find it.
    find_file(SDL2_DLL SDL2.dll PATHS ${SDL2_LIBDIR}/../bin ${SDL2_DIR}/bin NO_DEFAULT_PATH)
    if(SDL2_DLL)
        install(FILES ${SDL2_DLL} DESTINATION bin)
    endif()
endif()

include(CPack)
