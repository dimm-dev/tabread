cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(tabread CXX)

if (WIN32)
# special for GTEXT
cmake_policy(SET CMP0074 NEW)
endif()

if (CMAKE_GENERATOR STREQUAL "")
if (MINGW)
  set(CMAKE_GENERATOR "MinGW Makefiles")
else()
  set(CMAKE_GENERATOR "Unix Makefiles")
endif()
  message("generator is set to ${CMAKE_GENERATOR}")
endif()

if ("${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "Build in source directory is not allowed. Please make another directory for build the project.")
endif()

enable_testing()

if (MSVC)
  set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

if (NOT CMAKE_SIZEOF_VOID_P GREATER 0)
  message(FATAL_ERROR "CMake fails to determine the bitness of the target platform. Please check your CMake and compiler installation. If you are cross-compiling then ensure that your CMake toolchain file correctly sets the compiler details.")
endif()

if (MINGW)
  if (EXISTS /mingw)
      list(APPEND CMAKE_INCLUDE_PATH /mingw)
  endif()
  if (EXISTS /mingw32)
      list(APPEND CMAKE_INCLUDE_PATH /mingw32)
  endif()
  if (EXISTS /mingw64)
      list(APPEND CMAKE_INCLUDE_PATH /mingw64)
  endif()
endif()

# set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Output directory for applications")
# set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Output directory for libraries")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (CMAKE_GENERATOR MATCHES "Makefiles" AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

if (ENABLE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

enable_testing()

# find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
find_program(CPPCHECK_NAME NAMES cppcheck)
if (NOT CPPCHECK_NAME STREQUAL "CPPCHECK_NAME-NOTFOUND")
  set(CMAKE_CXX_CPPCHECK "${CPPCHECK_NAME}")
endif()

add_compile_options(-Wall -Wextra)

add_subdirectory(actions)
add_subdirectory(imgfilter)
add_subdirectory(text)
if (WIN32)
add_subdirectory(w32wrap)
endif()
add_subdirectory(tabread)
