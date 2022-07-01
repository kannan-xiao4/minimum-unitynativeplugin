# setup

cmake_minimum_required(VERSION 3.18)

set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0" CACHE STRING "Minimum iOS deployment version")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

project(minimum
  LANGUAGES C CXX ASM
)

# Define flags to determine the build target platform
# Windows, Linux, macOS, iOS
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(Windows TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  set(iOS TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(Linux TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(macOS TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
  set(Android TRUE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(XCODE)
  if(${XCODE_VERSION} VERSION_GREATER_EQUAL 12.0.0 AND
    ${XCODE_VERSION} VERSION_LESS 11.0.0
  )
    message(FATAL_ERROR
      "The required Xcode version is 11.0 or higher and 12.0 or higher is not supported.")
  endif()
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# build target
add_library(MinimumPlugin SHARED)

target_include_directories(MinimumPlugin
  SYSTEM
  PUBLIC
  ${CMAKE_SOURCE_DIR}/unity
)

target_sources(MinimumPlugin
  PRIVATE
  MinimumPlugin.cpp
  ScopedProfiler.h
  ScopedProfiler.cpp
)

if(Windows)
  set(PROJECT_BINARY_DIR
    "${CMAKE_SOURCE_DIR}/../SampleProject/Assets/Plugins/x86_64")
elseif(macOS)
  set(PROJECT_BINARY_DIR
    "${CMAKE_SOURCE_DIR}/../SampleProject/Assets/Plugins/macOS")
elseif(Linux)
  set(PROJECT_BINARY_DIR
    "${CMAKE_SOURCE_DIR}/../SampleProject/Assets/Plugins/x86_64")
elseif(iOS)
  set(PROJECT_BINARY_DIR
    "${CMAKE_SOURCE_DIR}/../SampleProject/Assets/Plugins/iOS")
elseif(Android)
  set(PROJECT_BINARY_DIR
    "${CMAKE_SOURCE_DIR}/../SampleProject/Assets/Plugins/Android")
endif()

# rename dll/framework filename
set_target_properties(MinimumPlugin
  PROPERTIES
  OUTPUT_NAME minimum
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}
)
