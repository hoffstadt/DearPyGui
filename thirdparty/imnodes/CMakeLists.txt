cmake_minimum_required(VERSION 3.15)

project(imnodes)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# determine whether this is a standalone project or included by other projects
if (NOT DEFINED IMNODES_STANDALONE_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(IMNODES_STANDALONE_PROJECT ON)
    else()
        set(IMNODES_STANDALONE_PROJECT OFF)
    endif ()
endif()

# cmake options
option(IMNODES_EXAMPLES "Build examples" ${IMNODES_STANDALONE_PROJECT})

# allow custom imgui target name since this can vary because imgui doesn't natively include a CMakeLists.txt
if(NOT DEFINED IMNODES_IMGUI_TARGET_NAME)
    find_package(imgui CONFIG REQUIRED)
    set(IMNODES_IMGUI_TARGET_NAME imgui::imgui)
endif()


if(MSVC)
    add_compile_definitions(SDL_MAIN_HANDLED)
    add_compile_options(/WX)
    # replace existing /W to avoid warning
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        add_compile_options(/W4)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Imnodes

add_library(imnodes)
target_sources(imnodes PRIVATE
    imnodes.h
    imnodes_internal.h
    imnodes.cpp)
target_include_directories(imnodes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imnodes PUBLIC ${IMNODES_IMGUI_TARGET_NAME})

# Example projects
if(IMNODES_EXAMPLES)

    find_package(SDL2 CONFIG REQUIRED)

    add_executable(colornode
        ${CMAKE_CURRENT_SOURCE_DIR}/imnodes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/color_node_editor.cpp)
    target_link_libraries(colornode imnodes SDL2::SDL2)
    if (APPLE)
        target_link_libraries(colornode "-framework OpenGL")
    elseif(MSVC)
        target_link_libraries(colornode "opengl32")
    else()
        target_link_libraries(colornode X11 Xext GL)
    endif()

    add_executable(multieditor
        ${CMAKE_CURRENT_SOURCE_DIR}/imnodes.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/example/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/multi_editor.cpp)
    target_link_libraries(multieditor imnodes SDL2::SDL2)
    if (APPLE)
        target_link_libraries(multieditor "-framework OpenGL")
    elseif(MSVC)
        target_link_libraries(multieditor "opengl32")
    else()
        target_link_libraries(multieditor X11 Xext GL)
    endif()

    add_executable(saveload
        ${CMAKE_CURRENT_SOURCE_DIR}/imnodes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/save_load.cpp)
    target_link_libraries(saveload imnodes SDL2::SDL2)
    if (APPLE)
        target_link_libraries(saveload "-framework OpenGL")
    elseif(MSVC)
        target_link_libraries(saveload "opengl32")
    else()
        target_link_libraries(saveload X11 Xext GL)
    endif()

    add_executable(hello
        ${CMAKE_CURRENT_SOURCE_DIR}/imnodes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/example/hello.cpp)
    target_link_libraries(hello imnodes SDL2::SDL2)
    if (APPLE)
        target_link_libraries(hello "-framework OpenGL")
    elseif(MSVC)
        target_link_libraries(hello "opengl32")
    else()
        target_link_libraries(hello X11 Xext GL)
    endif()
endif()
