project(Memo)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/libs/report")

set(Memo_SRCS memo.cpp memohelper.cpp)
QT4_WRAP_CPP(Memo_SRCS memo.h memohelper.h)
QT4_ADD_RESOURCES(Memo_SRCS memo.qrc)
QT4_WRAP_UI(Memo_SRCS memohelper.ui)

add_library(Memo SHARED ${Memo_SRCS})
add_dependencies(Memo Report)
link_directories( "${CMAKE_BINARY_DIR}/libs/report" )
target_link_libraries(Memo Report ${QT_LIBRARIES})

install(TARGETS Memo ${INSTALL_PLUGINS_DEFAULT_ARGS})
