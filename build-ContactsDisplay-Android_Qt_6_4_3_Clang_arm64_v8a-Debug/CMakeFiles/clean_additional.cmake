# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appContactsDisplay_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appContactsDisplay_autogen.dir/ParseCache.txt"
  "appContactsDisplay_autogen"
  )
endif()
