project(OMSimulatorLIb)

set(OM_SIM_LIB_SOURCES oms_logging.cpp oms_model.cpp oms_resultfile.cpp OMSimulator.cpp Settings.cpp)

# Shared library version
# add_library(omsim_dynamic SHARED ${OM_SIM_LIB_SOURCES})
# install(TARGETS omsim_dynamic DESTINATION lib)

include_directories(${FMILibrary_INCLUDEDIR})

# Static library version
add_library(omsim STATIC ${OM_SIM_LIB_SOURCES})
target_link_libraries(omsim)
# set_property(TARGET omsim PROPERTY CXX_STANDARD 11)
install(TARGETS omsim DESTINATION lib)

# Move header file
install(FILES OMSimulator.h DESTINATION include)
