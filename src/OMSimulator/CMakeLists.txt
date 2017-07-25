project(OMSimulator)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

include_directories(../OMSimulatorLib)
include_directories(../OMSimulatorLua)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LUALibrary_INCLUDEDIR})

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${FMILibrary_LIBRARYDIR})
link_directories(${LUALibrary_LIBRARYDIR})
link_directories(${CVODELibrary_LIBRARYDIR})

add_executable(OMSimulator main.cpp Options.cpp)

IF (WIN32)
  target_link_libraries(OMSimulator OMSimulatorLib fmilib_shared sundials_cvode sundials_nvecserial ${CMAKE_DL_LIBS} ${Boost_LIBRARIES} lua)
ELSE ()
  target_link_libraries(OMSimulator OMSimulatorLib fmilib_shared sundials_cvode sundials_nvecserial ${CMAKE_DL_LIBS} ${Boost_LIBRARIES} lua dl)
ENDIF ()

# set_property(TARGET OMSimulator PROPERTY CXX_STANDARD 11)

install(TARGETS OMSimulator DESTINATION bin)
