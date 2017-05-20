

set(command "C:/Program Files/CMake/bin/cmake.exe;--build;.;--config;Debug")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "F:/Workspace/Engine/Source/Engine/3rdParty/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-build-out.log"
  ERROR_FILE "F:/Workspace/Engine/Source/Engine/3rdParty/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  F:/Workspace/Engine/Source/Engine/3rdParty/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gtest build command succeeded.  See also F:/Workspace/Engine/Source/Engine/3rdParty/assimp-3.3.1/test/gtest/src/gtest-stamp/gtest-build-*.log")
  message(STATUS "${msg}")
endif()
