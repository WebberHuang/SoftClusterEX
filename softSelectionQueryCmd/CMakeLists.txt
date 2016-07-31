cmake_minimum_required(VERSION 3.0)
project(softSelectionQuery)

set(CMAKE_BUILD_TYPE Release)

# Add path to Chad Vernons FindMaya.cmake modules
# https://github.com/chadmv/cgcmake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Setting default building version
set(MAYA_VERSION 2016 CACHE STRING "Maya version")

# Setting plugin version
set(MAYA_PLUGIN_MAJOR "1")
set(MAYA_PLUGIN_MINOR "5")
set(MAYA_PLUGIN_PATCH "0")
set(SOURCE_FILE_DIR ${PROJECT_SOURCE_DIR}/src)

configure_file (
    "${SOURCE_FILE_DIR}/version.h.in"
    "${SOURCE_FILE_DIR}/version.h"
)

# Customize maya sdk directory
if(DEFINED MAYA_SDK_BASE_DEFAULT)
    #set(MAYA_SDK_BASE_DEFAULT /Volumes/Data/Libs/MayaSDK)

    if(WIN32)
        # Windows
        set(MAYA_SDK_BASE_SUFFIX Windows)
        set(MAYA_INSTALL_BASE_DEFAULT ${MAYA_SDK_BASE_DEFAULT}/${MAYA_SDK_BASE_SUFFIX})
    elseif(APPLE)
        # Apple
        set(MAYA_SDK_BASE_SUFFIX MacOS)
        set(MAYA_INSTALL_BASE_DEFAULT ${MAYA_SDK_BASE_DEFAULT}/${MAYA_SDK_BASE_SUFFIX})
    else()
        # Linux
        set(MAYA_SDK_BASE_SUFFIX Linux)
        set(MAYA_INSTALL_BASE_DEFAULT ${MAYA_SDK_BASE_DEFAULT}/${MAYA_SDK_BASE_SUFFIX})
    endif()
endif()

# Variables for output folder
set(BUILD_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/build)

# Source files
set(SOURCE_FILES
    src/pluginMain.cpp
    src/softSelectionQueryCmd.cpp
    src/softSelectionQueryCmd.h
    src/version.h)

# Find Maya and declare variables:
#  MAYA_FOUND          Defined if a Maya installation has been detected
#  MAYA_EXECUTABLE     Path to Maya's executable
#  MAYA_<lib>_FOUND    Defined if <lib> has been found
#  MAYA_<lib>_LIBRARY  Path to <lib> library
#  MAYA_INCLUDE_DIR    Path to the devkit's include directories
#  MAYA_LIBRARIES      All the Maya libraries
find_package(Maya REQUIRED)

find_package(OpenGL REQUIRED)

# Output folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_BASE_DIR})

# Build options
include_directories(${SOURCE_FILE_DIR} ${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

MAYA_PLUGIN(${PROJECT_NAME})