project(OMSimulatorLib)

set(OMSIMULATORLIB_SOURCES oms_logging.cpp oms_model.cpp oms_resultfile.cpp OMSimulator.cpp Settings.cpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/oms_version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/oms_version.cpp" @ONLY)
list(APPEND OMSIMULATORLIB_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/oms_version.cpp")

# Shared library version
# add_library(omsim_dynamic SHARED ${OMSIMULATORLIB_SOURCES})
# install(TARGETS omsim_dynamic DESTINATION lib)

include_directories(${FMILibrary_INCLUDEDIR})

# Static library version
add_library(libOMSimulator STATIC ${OMSIMULATORLIB_SOURCES})
SET_TARGET_PROPERTIES(libOMSimulator PROPERTIES OUTPUT_NAME OMSimulator)
target_link_libraries(libOMSimulator)
# set_property(TARGET libOMSimulator PROPERTY CXX_STANDARD 11)

install(TARGETS libOMSimulator DESTINATION lib)
install(FILES OMSimulator.h DESTINATION include)
