project(OMSimulator)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(../OMSimulatorLib)
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS})

add_executable(OMSimulator main.cpp)

target_link_libraries(OMSimulator OMSimulatorLib fmilib_shared ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})

# set_property(TARGET OMSimulator PROPERTY CXX_STANDARD 11)

install(TARGETS OMSimulator DESTINATION bin)
