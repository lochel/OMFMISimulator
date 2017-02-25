project(luaOMSimulator C)

set(LUAOMSIMULATORLIB_SOURCES luaOMSimulator.c)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR} ../library)

link_directories(${FMILibrary_LIBRARYDIR})

# Shared library version
add_library(luaOMSimulator SHARED ${LUAOMSIMULATORLIB_SOURCES})
target_link_libraries(luaOMSimulator OMSimulator_shared fmilib)

install(TARGETS luaOMSimulator DESTINATION lib)
