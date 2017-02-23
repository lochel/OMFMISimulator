project(OMSimulatorExe)

include_directories(../library)
link_directories(${FMILibrary_LIBRARYDIR} ${Boost_LIBRARY_DIRS})

add_executable(OMSimulator main.cpp)

if (MSVC)
  # FIXME get "unresolved external symbol" error if linking to static fmilib
  target_link_libraries(OMSimulator libOMSimulator fmilib_shared ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})
else (MSVC)
  target_link_libraries(OMSimulator libOMSimulator fmilib ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})
endif (MSVC)

# set_property(TARGET OMSimulator PROPERTY CXX_STANDARD 11)

install(TARGETS OMSimulator DESTINATION bin)
