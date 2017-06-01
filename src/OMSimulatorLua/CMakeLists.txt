project(OMSimulatorLua C)

find_package(Lua REQUIRED)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(${LUA_INCLUDE_DIR} ../OMSimulatorLib)
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS} ${LUA_LIBRARY_DIR})

# Shared library version
add_library(OMSimulatorLua SHARED OMSimulatorLua.c)
IF (WIN32)
  target_link_libraries(OMSimulatorLua OMSimulatorLib_shared fmilib lua53)
ELSE ()
  target_link_libraries(OMSimulatorLua OMSimulatorLib_shared fmilib)
ENDIF ()

install(TARGETS OMSimulatorLua DESTINATION lib)
