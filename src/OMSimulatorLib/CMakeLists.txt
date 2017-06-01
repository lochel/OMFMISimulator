project(OMSimulatorLib)

set(OMSIMULATORLIB_SOURCES Logging.cpp FMUWrapper.cpp CompositeModel.cpp Resultfile.cpp DirectedGraph.cpp OMSimulator.cpp Settings.cpp Variable.cpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/Version.cpp" @ONLY)
list(APPEND OMSIMULATORLIB_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/Version.cpp")

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMILibrary_INCLUDEDIR})
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS})

# Shared library version
add_library(OMSimulatorLib_shared SHARED ${OMSIMULATORLIB_SOURCES})
SET_TARGET_PROPERTIES(OMSimulatorLib_shared PROPERTIES OUTPUT_NAME OMSimulatorLib)
target_link_libraries(OMSimulatorLib_shared fmilib_shared ${Boost_LIBRARIES})
install(TARGETS OMSimulatorLib_shared DESTINATION lib)

# Static library version
add_library(OMSimulatorLib STATIC ${OMSIMULATORLIB_SOURCES})
SET_TARGET_PROPERTIES(OMSimulatorLib PROPERTIES OUTPUT_NAME OMSimulatorLib)
# set_property(TARGET OMSimulatorLib PROPERTY CXX_STANDARD 11)
install(TARGETS OMSimulatorLib DESTINATION lib)

install(FILES OMSimulator.h DESTINATION include)
