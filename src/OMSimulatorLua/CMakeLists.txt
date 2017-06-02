project(OMSimulatorLua C)

#find_package(Lua REQUIRED)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(${LUALibrary_INCLUDEDIR} ../OMSimulatorLib)
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS} ${LUALibrary_LIBRARYDIR})

# Shared library version
add_library(OMSimulatorLua SHARED OMSimulatorLua.c)
IF (WIN32)
  target_link_libraries(OMSimulatorLua OMSimulatorLib_shared fmilib_shared lua)
ELSE ()
  target_link_libraries(OMSimulatorLua OMSimulatorLib_shared fmilib_shared)
ENDIF ()

install(TARGETS OMSimulatorLua DESTINATION lib)
