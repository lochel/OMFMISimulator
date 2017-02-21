project(OMSimulator)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_BUILD_TYPE DEBUG)

# Don't allow in-source build
IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "No in-source builds supported. Change to 'build' sub-directory and do 'cmake ..'.")
ENDIF()

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build CACHE PATH "Library installation path (don't change)" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

##########################
# Configuring for boost "FindBoost.cmake"
# FIXME BTH arbitrary settings for boost. There is more what can be set. What do we want?
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
# Boost v1.54 is the boost shipped with my Ubuntu 14.04, lower versions may work, too.
find_package(Boost 1.54.0
  COMPONENTS system filesystem
  REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Set where your FMILibrary is installed here
set(FMILibrary_ROOT ${PROJECT_SOURCE_DIR}/3rdParty/FMIL/install)
find_package(FMILibrary REQUIRED)

##########################
# Add project modules
add_subdirectory(library)
add_subdirectory(simulator)

##########################
# TODO Add tests
