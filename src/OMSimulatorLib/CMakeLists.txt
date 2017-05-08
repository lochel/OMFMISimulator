project(OMSimulatorLib)

set(OMSIMULATORLIB_SOURCES Logging.cpp oms_fmu.cpp oms_model.cpp oms_resultfile.cpp DirectedGraph.cpp OMSimulator.cpp Settings.cpp Variable.cpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/oms_version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/oms_version.cpp" @ONLY)
list(APPEND OMSIMULATORLIB_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/oms_version.cpp")

include_directories(${FMILibrary_INCLUDEDIR})
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS})

# Shared library version
add_library(OMSimulator_shared SHARED ${OMSIMULATORLIB_SOURCES})
target_link_libraries(OMSimulator_shared fmilib ${Boost_LIBRARIES})
install(TARGETS OMSimulator_shared DESTINATION lib)

# Static library version
add_library(OMSimulatorLib STATIC ${OMSIMULATORLIB_SOURCES})
SET_TARGET_PROPERTIES(OMSimulatorLib PROPERTIES OUTPUT_NAME OMSimulator)
# set_property(TARGET OMSimulatorLib PROPERTY CXX_STANDARD 11)

install(TARGETS OMSimulatorLib DESTINATION lib)
install(FILES OMSimulator.h DESTINATION include)
