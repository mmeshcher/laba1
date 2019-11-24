#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Json" for configuration "Debug"
set_property(TARGET Json APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Json PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libJsond.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Json )
list(APPEND _IMPORT_CHECK_FILES_FOR_Json "${_IMPORT_PREFIX}/lib/libJsond.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
