cmake_minimum_required(VERSION 3.27)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

project(bbui)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)

add_library(bbui SHARED ${SRC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad.c ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image.cpp)
if(WIN32)
    target_link_libraries(bbui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/lib-vc2019/glfw3.lib)
else()
    target_link_libraries(bbui PRIVATE glfw pthread)
endif()
target_include_directories(bbui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(bbui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set_property(GLOBAL PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET bbui PROPERTY CXX_STANDARD 20)
if(WIN32)
    set(CMAKE_C_COMPILER "cl.exe")
    set(CMAKE_CXX_COMPILER "cl.exe")

    add_compile_options("/ZI")
    add_link_options("/DEBUG")
else()
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

    add_compile_options("-g")
    add_link_options("-g")
    add_compile_options("-O3")
    add_compile_options(
        -fno-delete-null-pointer-checks
        -Wno-tautological-undefined-compare
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-missing-field-initializers
    )
endif()

macro(get_file_content filename content_var)
    file(READ ${filename} temporary_var)
    string(REGEX REPLACE "#include *\".+\"\n" "" temporary_var "${temporary_var}")
    string(REGEX REPLACE "#pragma *[^\n]*\n" "" temporary_var "${temporary_var}")
    string(APPEND ${content_var} "${temporary_var}")
endmacro()

set(COMBINED_FILE "/*
 * BBUI interface rendering toolkit.
 * Copyright (C) 2026  cassette costen

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if __cplusplus < 202002L
#error BBUI requires C++20 to be compiled
#endif

#pragma region BBUI_DECLARATIONS\n\n")
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/inc/window.h COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/inc/rendering.h COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/inc/canvas.h COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/inc/components.h COMBINED_FILE)
string(APPEND COMBINED_FILE "\n\n#pragma endregion BBUI_DECLARATIONS

#pragma region BBUI_IMPLEMENTATIONS
#if defined(BBUI_IMPLEMENTATION)\n\n")
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/src/backend_opengl.cpp COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/src/surface_opengl.cpp COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/src/default_textures.cpp COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/src/renderer_primitives.cpp COMBINED_FILE)
get_file_content(${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp COMBINED_FILE)
string(APPEND COMBINED_FILE "\n\n#endif
#pragma endregion BBUI_IMPLEMENTATIONS\n")

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/export/bbui_headeronly.h "${COMBINED_FILE}")

if(WIN32)
    set(PLATFORM Windows)
else()
    set(PLATFORM Linux)
endif()
set(TAR_OUTPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/export/bbui_prebuilt_${PLATFORM}.zip"
)

add_custom_target(bbui_export
    DEPENDS bbui
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/inc/ ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bbui> ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/staging ${CMAKE_COMMAND} -E tar cfv ${TAR_OUTPUT} --format=zip .
)