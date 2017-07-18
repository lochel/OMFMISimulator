project(OMSimulator)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(../OMSimulatorLib)

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${FMILibrary_LIBRARYDIR})
link_directories(${CVODELibrary_LIBRARYDIR})

add_executable(OMSimulator main.cpp)

target_link_libraries(OMSimulator OMSimulatorLib fmilib_shared sundials_cvode sundials_nvecserial ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})

# set_property(TARGET OMSimulator PROPERTY CXX_STANDARD 11)

install(TARGETS OMSimulator DESTINATION bin)
