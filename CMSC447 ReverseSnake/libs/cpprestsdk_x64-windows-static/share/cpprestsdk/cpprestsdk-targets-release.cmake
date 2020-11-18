#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cpprestsdk::cpprest" for configuration "Release"
set_property(TARGET cpprestsdk::cpprest APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cpprestsdk::cpprest PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/cpprest_2_10.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS cpprestsdk::cpprest )
list(APPEND _IMPORT_CHECK_FILES_FOR_cpprestsdk::cpprest "${_IMPORT_PREFIX}/lib/cpprest_2_10.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
