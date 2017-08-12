project(OMSimulatorModelica)

set(OMSimulatorModelicaResources ${CMAKE_CURRENT_SOURCE_DIR}/OMSimulator/Resources)

install(FILES ../OMSimulatorLib/OMSimulator.h DESTINATION ${OMSimulatorModelicaResources}/Include)
install(FILES ../OMSimulatorLib/Types.h DESTINATION ${OMSimulatorModelicaResources}/Include)

# FIXME BTH: It is not valid to directly reference the OMSimulatorLib_shared target,
# but the workaround below might not always copy the latest build of the library
# Notice that using the lib from ${CMAKE_BINARY_DIR}.. directly is not working for me,
# possibly because its RPATH is different to the one which is set for "installed" libs??
if ( (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (NOT M32_FLAG) )
  if (UNIX)
    # install(TARGET OMSimulatorLib_shared DESTINATION ${OMSimulatorModelicaResources}/Library/linux64) # Not valid, since target defined in another file
    install(FILES ${CMAKE_SOURCE_DIR}/install/lib/libOMSimulatorLib.so DESTINATION ${OMSimulatorModelicaResources}/Library/linux64)
  else (UNIX)
    install(FILES ${CMAKE_SOURCE_DIR}/install/lib/OMSimulatorLib.dll DESTINATION ${OMSimulatorModelicaResources}/Library/win64)
    install(FILES ${CMAKE_SOURCE_DIR}/install/lib/OMSimulatorLib.lib DESTINATION ${OMSimulatorModelicaResources}/Library/win64)
  endif (UNIX)
elseif ( (CMAKE_SIZEOF_VOID_P EQUAL 4) OR M32_FLAG)
  if (UNIX)
    install(FILES ${CMAKE_SOURCE_DIR}/install/lib/libOMSimulatorLib.so DESTINATION ${OMSimulatorModelicaResources}/Library/linux32)
  else (UNIX)
    install(FILES ${CMAKE_SOURCE_DIR}/install/lib/OMSimulatorLib.dll DESTINATION ${OMSimulatorModelicaResources}/Library/win32)
    install(FILES ${CMAKE_SOURCE_DIR}/install/lib/OMSimulatorLib.lib DESTINATION ${OMSimulatorModelicaResources}/Library/win32)
  endif (UNIX)
else ( (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (NOT M32_FLAG) )
  message(SEND_ERROR "Uups. Neither 64, nor 32 bit??")
endif ( (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (NOT M32_FLAG) )
