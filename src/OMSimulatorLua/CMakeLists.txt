project(OMSimulatorLua C)

find_package(Lua REQUIRED)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(${LUA_INCLUDE_DIR} ../OMSimulatorLib)
link_directories(${FMILibrary_LIBRARYDIR})

# Shared library version
add_library(OMSimulatorLua SHARED OMSimulatorLua.c)
target_link_libraries(OMSimulatorLua OMSimulatorLib_shared fmilib)

install(TARGETS OMSimulatorLua DESTINATION lib)
