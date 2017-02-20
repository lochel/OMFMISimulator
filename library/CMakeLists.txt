message(STATUS "WRITING BUILD FILES FOR 'omsl' library")
#message(STATUS "${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}")

set(libSrcsOmsl oms_logging.cpp oms_model.cpp  oms_resultfile.cpp OMSimulator.cpp)

# Shared library version
add_library(omsl SHARED ${libSrcsOmsl})
add_dependencies(omsl FMILibrary)
install(TARGETS omsl DESTINATION lib)

# Static library version
add_library(omsl_static STATIC ${libSrcsOmsl})
add_dependencies(omsl_static FMILibrary)
install(TARGETS omsl_static DESTINATION lib)

# Move header file
install(FILES ../include/omsl.h DESTINATION include)
