project(OMFitLib)

set(CMAKE_INSTALL_RPATH "$ORIGIN") # BTH needed?
set(CMAKE_VERBOSE_MAKEFILE ON)

# if (BUILD_SHARED_LIBS) # BTH needed?
#   remove_definitions(-DCERES_BUILDING_SHARED_LIBRARY)
# endif()

include_directories(${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/OMSimulatorLib
  ${FMILibrary_INCLUDEDIR}
  ${CERES_INCLUDE_DIRS}
)
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS})

set(OMFITLIB_SOURCES OMFit.cpp FitModel.cpp)
set(OMFITLIB_LIBRARIES OMSimulatorLib ${FMILibrary_LIBRARY} sundials_cvode sundials_nvecserial ${CMAKE_DL_LIBS} ${Boost_LIBRARIES} ${CERES_LIBRARIES})
set(OMFITLIB_LIBS2 OMSimulatorLib_shared fmilib_shared sundials_cvode sundials_nvecserial)

# Shared library version
add_library(OMFit_shared SHARED ${OMFITLIB_SOURCES})
set_property(TARGET OMFit_shared PROPERTY CXX_STANDARD 11)
set_target_properties(OMFit_shared PROPERTIES OUTPUT_NAME OMFit)
target_link_libraries(OMFit_shared ${OMFITLIB_LIBRARIES})
install(TARGETS OMFit_shared DESTINATION lib)

# Static library version
add_library(OMFit STATIC ${OMFITLIB_SOURCES})
set_property(TARGET OMFit PROPERTY CXX_STANDARD 11)
set_target_properties(OMFit PROPERTIES POSITION_INDEPENDENT_CODE ON)
install(TARGETS OMFit DESTINATION lib)

install(FILES OMFit.h DESTINATION include)
