project(OMSimulator)
cmake_minimum_required(VERSION 2.8.12)
enable_testing()
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
# Third party projects
# Using "add_custom_command" here. Tried with "ExternalProject_Add", but couldn't get the build run reliably in ./3rdParty/FMIL/.
# https://coderwall.com/p/y3zzbq/use-cmake-enabled-libraries-in-your-cmake-project
# Resorted to the code below
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/build-fmil
  COMMAND cmake -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/build-fmil
)
add_custom_target(
  FMILibraryBuildDir
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/build-fmil
)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/build-fmil PROPERTIES GENERATED TRUE)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/install/lib/libfmilib.a
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/install/include/fmilib.h
    COMMAND cmake ..
    COMMAND make
    COMMAND make install
    DEPENDS FMILibraryBuildDir
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/build-fmil
)
add_custom_target(
     FMILibrary
     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/install/lib/libfmilib.a
     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/install/include/fmilib.h
)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/install/include/fmilib.h PROPERTIES GENERATED TRUE)
set(ffmilib ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMIL/install/lib/libfmilib.a)
add_library(fmilib STATIC IMPORTED GLOBAL)
set_property(TARGET fmilib PROPERTY IMPORTED_LOCATION ${ffmilib})
add_dependencies(fmilib FMILibrary)

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


##########################
# Project wide include directories
include_directories(3rdParty/FMIL/install/include)
include_directories(simulator)
include_directories(library)

##########################
# Add project modules
add_subdirectory(library)
add_subdirectory(simulator)

##########################
# TODO Add tests
