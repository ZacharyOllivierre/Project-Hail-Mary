cmake_minimum_required(VERSION 3.20)
project(Hail LANGUAGES C CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

if(WIN32)
    set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/include")

    if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR
            "The bundled MSVC SDL2 libraries are x64, but the current generator platform is Win32. "
            "Please reconfigure CMake with a 64-bit generator, for example: cmake -S . -B build -A x64"
        )
    endif()

    if(NOT MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR
            "The bundled MinGW SDL2 libraries are x64, but the current toolchain is 32-bit. "
            "Please reconfigure CMake with a 64-bit MinGW toolchain."
        )
    endif()

    include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

    if(MSVC)
        set(SDL2_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/lib/msvc")
        set(SDL2_LIBRARIES
            "${SDL2_LIB_DIR}/SDL2.lib"
            "${SDL2_LIB_DIR}/SDL2_net.lib"
            "${SDL2_LIB_DIR}/SDL2_gfx.lib"
            "${SDL2_LIB_DIR}/SDL2_image.lib"
            "${SDL2_LIB_DIR}/SDL2_mixer.lib"
            "${SDL2_LIB_DIR}/SDL2_ttf.lib"
        )
        set(SDL2_RUNTIME_FILES
            "${SDL2_LIB_DIR}/SDL2.dll"
            "${SDL2_LIB_DIR}/SDL2_net.dll"
            "${SDL2_LIB_DIR}/SDL2_gfx.dll"
            "${SDL2_LIB_DIR}/SDL2_image.dll"
            "${SDL2_LIB_DIR}/SDL2_mixer.dll"
            "${SDL2_LIB_DIR}/SDL2_ttf.dll"
        )
    else()
        set(SDL2_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/lib/mingw")
        set(SDL2_LIBRARIES
            "${SDL2_LIB_DIR}/libSDL2.dll.a"
            "${SDL2_LIB_DIR}/libSDL2_net.dll.a"
            "${SDL2_LIB_DIR}/libSDL2_gfx.dll.a"
            "${SDL2_LIB_DIR}/libSDL2_image.dll.a"
            "${SDL2_LIB_DIR}/libSDL2_mixer.dll.a"
            "${SDL2_LIB_DIR}/libSDL2_ttf.dll.a"
        )
        set(SDL2_RUNTIME_FILES
            "${SDL2_LIB_DIR}/SDL2.dll"
            "${SDL2_LIB_DIR}/SDL2_net.dll"
            "${SDL2_LIB_DIR}/SDL2_gfx.dll"
            "${SDL2_LIB_DIR}/SDL2_image.dll"
            "${SDL2_LIB_DIR}/SDL2_mixer.dll"
            "${SDL2_LIB_DIR}/SDL2_ttf.dll"
        )
    endif()

    foreach(SDL2_RUNTIME_FILE IN LISTS SDL2_RUNTIME_FILES)
        if(EXISTS "${SDL2_RUNTIME_FILE}")
            file(COPY "${SDL2_RUNTIME_FILE}" DESTINATION "${CMAKE_BINARY_DIR}")
        endif()
    endforeach()
elseif(APPLE)
    set(SDL2_SEARCH_ROOTS /opt/homebrew /usr/local)

    find_path(
        SDL2_INCLUDE_DIR
        SDL.h
        HINTS ${SDL2_SEARCH_ROOTS}
        PATH_SUFFIXES include/SDL2 include
        REQUIRED
    )
    find_library(
        SDL2_LIBRARY
        SDL2
        HINTS ${SDL2_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )
    find_library(
        SDL2_IMAGE_LIBRARY
        SDL2_image
        HINTS ${SDL2_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )
    find_library(
        SDL2_NET_LIBRARY
        SDL2_net
        HINTS ${SDL2_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )
    find_library(
        SDL2_MIXER_LIBRARY
        SDL2_mixer
        HINTS ${SDL2_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )
    find_library(
        SDL2_TTF_LIBRARY
        SDL2_ttf
        HINTS ${SDL2_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )

    set(SDL2_LIBRARIES
        ${SDL2_LIBRARY}
        ${SDL2_NET_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY}
    )

    include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    pkg_check_modules(SDL2_NET REQUIRED IMPORTED_TARGET SDL2_net)
    pkg_check_modules(SDL2_GFX REQUIRED IMPORTED_TARGET SDL2_gfx)
    pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

    set(SDL2_INCLUDE_DIR
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )
    list(REMOVE_DUPLICATES SDL2_INCLUDE_DIR)
    include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

    set(SDL2_LIBRARIES
        PkgConfig::SDL2
        PkgConfig::SDL2_NET
        PkgConfig::SDL2_GFX
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_MIXER
        PkgConfig::SDL2_TTF
    )
endif()


add_subdirectory(application)
add_subdirectory(engine)
add_subdirectory(gameplay)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(thirdparty/imgui)

add_executable(Hail "main.cpp")

target_include_directories(Hail PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nlohmann
)

target_link_libraries(Hail app_lib gameplay_lib engine_lib imgui_lib ${SDL2_LIBRARIES})
