#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "composable_node_demo::client_node_component" for configuration ""
set_property(TARGET composable_node_demo::client_node_component APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(composable_node_demo::client_node_component PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libclient_node_component.so"
  IMPORTED_SONAME_NOCONFIG "libclient_node_component.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS composable_node_demo::client_node_component )
list(APPEND _IMPORT_CHECK_FILES_FOR_composable_node_demo::client_node_component "${_IMPORT_PREFIX}/lib/libclient_node_component.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
