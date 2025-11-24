# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\EC535_Lab5_game_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\EC535_Lab5_game_autogen.dir\\ParseCache.txt"
  "EC535_Lab5_game_autogen"
  )
endif()
