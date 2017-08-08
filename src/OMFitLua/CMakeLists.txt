project(OMFitLua C)

#find_package(Lua REQUIRED)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(${CMAKE_SOURCE_DIR}/src
${LUALibrary_INCLUDEDIR}
${CMAKE_SOURCE_DIR}/src/OMSimulatorLib
${CMAKE_SOURCE_DIR}/src/OMFitLib)

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${FMILibrary_LIBRARYDIR})
link_directories(${LUALibrary_LIBRARYDIR})
link_directories(${CVODELibrary_LIBRARYDIR})

# Shared library version
add_library(OMFitLua SHARED OMFitLua.c)
IF (WIN32)
  target_link_libraries(OMFitLua OMFit_shared OMSimulatorLib_shared fmilib_shared sundials_cvode sundials_nvecserial lua)
ELSE ()
  target_link_libraries(OMFitLua OMFit_shared OMSimulatorLib_shared fmilib_shared sundials_cvode sundials_nvecserial)
ENDIF ()

install(TARGETS OMFitLua DESTINATION lib)
