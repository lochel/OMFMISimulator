message(STATUS "WRITING BUILD FILES FOR OMSimulator executable")
message(STATUS "${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}")

add_executable(OMSimulator main.cpp)
target_link_libraries(OMSimulator omsim fmilib dl ${Boost_LIBRARIES})

install(TARGETS OMSimulator DESTINATION bin)
