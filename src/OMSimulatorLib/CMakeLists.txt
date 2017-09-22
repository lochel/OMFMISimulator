project(OMSimulatorLib)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

set(OMSIMULATORLIB_SOURCES Logging.cpp FMUWrapper.cpp CompositeModel.cpp ResultReader.cpp CSVReader.cpp MatReader.cpp ResultWriter.cpp CSVWriter.cpp MATWriter.cpp MatVer4.cpp DirectedGraph.cpp OMSimulator.cpp GlobalSettings.cpp Settings.cpp Variable.cpp Clock.cpp Clocks.cpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/Version.cpp" @ONLY)
list(APPEND OMSIMULATORLIB_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/Version.cpp")

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FMILibrary_INCLUDEDIR})
include_directories(${CVODELibrary_INCLUDEDIR})
include_directories(${PUGIXML_INCLUDEDIR})

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${FMILibrary_LIBRARYDIR})
link_directories(${CVODELibrary_LIBRARYDIR})

# Shared library version
add_library(OMSimulatorLib_shared SHARED ${OMSIMULATORLIB_SOURCES})
set_target_properties(OMSimulatorLib_shared PROPERTIES OUTPUT_NAME OMSimulatorLib)
target_link_libraries(OMSimulatorLib_shared fmilib_shared sundials_cvode sundials_nvecserial ${Boost_LIBRARIES})
install(TARGETS OMSimulatorLib_shared DESTINATION lib)

# Static library version
add_library(OMSimulatorLib STATIC ${OMSIMULATORLIB_SOURCES})
set_target_properties(OMSimulatorLib PROPERTIES OUTPUT_NAME OMSimulatorLib)
# set_property(TARGET OMSimulatorLib PROPERTY CXX_STANDARD 11)
set_target_properties(OMSimulatorLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
install(TARGETS OMSimulatorLib DESTINATION lib)

install(FILES OMSimulator.h Types.h DESTINATION include)
