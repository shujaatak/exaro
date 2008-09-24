# - Find eXaro
# Find the native eXaro includes and library
#
#  EXARO_INCLUDE_DIR - where to find qrencode.h, etc.
#  EXARO_LIBRARIES   - List of libraries when using QRencode.
#  EXARO_FOUND       - True if QRencode found.


IF (EXARO_INCLUDE_DIR)
  # Already in cache, be silent
  SET(EXARO_FIND_QUIETLY TRUE)
ENDIF (EXARO_INCLUDE_DIR)

FIND_PATH(EXARO_INCLUDE_DIR reportengine.h)

FIND_LIBRARY(EXARO_LIBRARY NAMES Report )

# handle the QUIETLY and REQUIRED arguments and set EXARO_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(eXaro DEFAULT_MSG EXARO_LIBRARY EXARO_INCLUDE_DIR)

IF(EXARO_FOUND)
  SET( EXARO_LIBRARIES ${EXARO_LIBRARY} )
ELSE(EXARO_FOUND)
  SET( EXARO_LIBRARIES )
ENDIF(EXARO_FOUND)

MARK_AS_ADVANCED( EXARO_LIBRARY EXARO_INCLUDE_DIR )
