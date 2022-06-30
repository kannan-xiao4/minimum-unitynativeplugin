# setup

cmake_minimum_required(VERSION 3.18)

set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0" CACHE STRING "Minimum iOS deployment version")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

project(minimum
  LANGUAGES C CXX ASM
)

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
)

# rename dll/framework filename
set_target_properties(MinimumPlugin
  PROPERTIES
    OUTPUT_NAME minimum
)
