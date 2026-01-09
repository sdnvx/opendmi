#
# OpenDMI: Cross-platform DMI/SMBIOS framework
# Copyright (c) 2025-2026, The OpenDMI contributors
#
# SPDX-License-Identifier: BSD-3-Clause
#
cmake_minimum_required(VERSION 3.25)

project(opendmi
        VERSION 0.1.2
        DESCRIPTION "Cross-platform DMI/SMBIOS framework"
        HOMEPAGE_URL https://github.com/sdnvx/opendmi
        LANGUAGES C CXX)

set(OPENDMI_VERSION "${CMAKE_PROJECT_VERSION}")

set(OPENDMI_VERSION_MAJOR "${CMAKE_PROJECT_VERSION_MAJOR}")
set(OPENDMI_VERSION_MINOR "${CMAKE_PROJECT_VERSION_MINOR}")
set(OPENDMI_VERSION_PATCH "${CMAKE_PROJECT_VERSION_PATCH}")

set(OPENDMI_CXX OFF CACHE BOOL "Enable C++ support(libopendmi++)")
set(OPENDMI_GOLANG OFF CACHE BOOL "Enable Go support (libopendmi-go)")
set(OPENDMI_PYTHON OFF CACHE BOOL "Enable Python support (libopendmi-python)")
set(OPENDMI_RUST OFF CACHE BOOL "Enable Rust support (libopendmi-rust)")
set(OPENDMI_DBUS OFF CACHE BOOL "Enable D-bus support (opendmi-dbus)")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(MSVC)
    add_compile_options(/std:c11 /W4)
    add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
else()
    add_compile_options(-Wall -Wextra)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        add_compile_options(-Wno-gnu-zero-variadic-macro-arguments -Wno-keyword-macro)
    endif()
endif()

if(UNIX)
    add_compile_definitions(_XOPEN_SOURCE _GNU_SOURCE)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options(-g -fno-omit-frame-pointer)

#   if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
#      "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
#       add_compile_options(--analyze -Xanalyzer -analyzer-output=text)
#   elseif("${CMALE_CXX_COMPILER_ID}" STREQUAL "GNU")
#       add_compile_options(-fanalyzer)
#   endif()
else()
    add_compile_options(-O2)
    add_compile_definitions(NDEBUG)
endif()

find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)

include(FindPackageEx)

find_package_ex(Curses ENABLE_CURSES)
find_package_ex(LibXml2 ENABLE_XML)
find_package_ex(LibYaml ENABLE_YAML)

include(CheckIncludeFile)

check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(share.h HAVE_SHARE_H)
check_include_file(io.h HAVE_IO_H)

enable_testing()

add_subdirectory(libopendmi)
add_subdirectory(opendmi)

if(ENABLE_CXX)
    message(STATUS "C++ support is enabled")
    add_subdirectory(libopendmi++)
else()
    message(STATUS "C++ support is disabled")
endif()

if(ENABLE_GOLANG)
    message(STATUS "GoLang support is enabled")
    add_subdirectory(libopendmi-go)
else()
    message(STATUS "GoLang suport is disabled")
endif()

if(ENABLE_PYTHON)
    message(STATUS "Python support is enabled")
    add_subdirectory(libopendmi-python)
else()
    message(STATUS "Python support is disabled")
endif()

if(ENABLE_RUST)
    message(STATUS "Rust support is enabled")
    add_subdirectory(libopendmi-rust)
else()
    message(STATUS "Rust support is disabled")
endif()

if(UNIX AND ENABLE_DBUS)
    message(STATUS "DBus support is enabled")
    add_subdirectory(opendmi-dbus)
else()
    message(STATUS "DBus support is disabled")
endif()

include(CPack)
