message(STATUS "WRITING BUILD FILES FOR 'omsim' library")
#message(STATUS "${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}")

set(libSrcsOmsim oms_logging.cpp oms_model.cpp  oms_resultfile.cpp OMSimulator.cpp)

# Shared library version
add_library(omsim_dynamic SHARED ${libSrcsOmsim})
add_dependencies(omsim_dynamic FMILibrary)
install(TARGETS omsim_dynamic DESTINATION lib)

# Static library version
add_library(omsim STATIC ${libSrcsOmsim})
add_dependencies(omsim FMILibrary)
install(TARGETS omsim DESTINATION lib)

# Move header file
install(FILES OMSimulator.h DESTINATION include)
