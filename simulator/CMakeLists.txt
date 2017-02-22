project(OMSimulatorExe)

include_directories(../library)
link_directories(${FMILibrary_LIBRARYDIR})

add_executable(OMSimulator main.cpp)
target_link_libraries(OMSimulator omsim fmilib dl ${Boost_LIBRARIES})
# set_property(TARGET OMSimulator PROPERTY CXX_STANDARD 11)

install(TARGETS OMSimulator DESTINATION bin)
