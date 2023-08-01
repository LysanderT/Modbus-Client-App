# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ModbusQT_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ModbusQT_autogen.dir/ParseCache.txt"
  "ModbusQT_autogen"
  )
endif()
