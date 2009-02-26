project(ToolbarInterface)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report" )

set(ToolbarInterface_SRCS toolbarinterface.cpp)
QT4_WRAP_CPP(ToolbarInterface_SRCS toolbarinterface.h)


add_library(ToolbarInterface STATIC ${ToolbarInterface_SRCS})

if (UNIX)
    set_target_properties(ToolbarInterface PROPERTIES COMPILE_FLAGS -fPIC)
endif(UNIX)

add_dependencies(ToolbarInterface Report)
