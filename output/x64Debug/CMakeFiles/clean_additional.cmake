# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\multiplayer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\multiplayer_autogen.dir\\ParseCache.txt"
  "multiplayer_autogen"
  )
endif()
