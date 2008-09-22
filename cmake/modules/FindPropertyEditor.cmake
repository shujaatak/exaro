# - Find PropertyEditor
# Find the native PropertyEditor includes and library
#
#  PE_INCLUDE_DIR - where to find propertyeditor.h, etc.
#  PE_LIBRARIES   - List of libraries when using PropertyEditor.
#  PE_FOUND       - True if PropertyEditor found.


IF (PE_INCLUDE_DIR)
  # Already in cache, be silent
  SET(PE_FIND_QUIETLY TRUE)
ENDIF (PE_INCLUDE_DIR)

FIND_PATH(PE_INCLUDE_DIR propertyeditor.h propertyinterface.h)

FIND_LIBRARY(PE_LIBRARY NAMES PropertyEditor)

# handle the QUIETLY and REQUIRED arguments and set PE_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PropertyEditor DEFAULT_MSG PE_LIBRARY PE_INCLUDE_DIR)

IF(PE_FOUND)
  SET( PE_LIBRARIES ${PE_LIBRARY} )
ELSE(PE_FOUND)
  SET( PE_LIBRARIES )
ENDIF(PE_FOUND)

MARK_AS_ADVANCED( PE_LIBRARY PE_INCLUDE_DIR )
